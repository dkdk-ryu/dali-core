//
// Created by adam.b on 31/03/17.
//

#include <dali/graphics/vulkan/graphics-adaptor.h>
#include <dali/graphics/vulkan/internal/graphics-surface-impl-base.h>
#include <dali/graphics/vulkan/internal/vulkan-adaptor.h>
#include <dali/graphics/vulkan/internal/vulkan-context.h>
#include <dali/graphics/vulkan/internal/vulkan-swapchain.h>
#include <dali/graphics/vulkan/internal/vulkan-command-buffer.h>
#include <dali/graphics/vulkan/surface/vulkan-surface.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{

VulkanContext::VulkanContext(const GraphicsAdaptor& adaptor, const GraphicsSurface& surface,
                             const ExtensionNameList& extensions)
: GraphicsContextImplBase(adaptor, surface, extensions)
{
  for(auto& name : extensions)
  {
    mEnabledExtensionStr.emplace_back(name);
    mEnabledExtensionCStr.emplace_back(mEnabledExtensionStr.back().c_str());
  }
}

bool VulkanContext::Initialise()
{
  auto adaptorImpl = mGraphicsAdaptor.GetObjectAs< VulkanAdaptor >();
  auto physDevice  = adaptorImpl->GetVkPhysicalDevice();
  auto features    = physDevice.getFeatures();
  // for each family allocate all possible queues when creating device
  std::vector< vk::DeviceQueueCreateInfo > queueInfoArray;

  // make a copy of used families and sort
  std::vector< int > familyIndices;

  auto newArray = std::vector< int >{adaptorImpl->GetQueueFamilyIndex(QueueType::GRAPHICS),
                                     adaptorImpl->GetQueueFamilyIndex(QueueType::COMPUTE),
                                     adaptorImpl->GetQueueFamilyIndex(QueueType::TRANSFER),
                                     adaptorImpl->GetQueueFamilyIndex(QueueType::SPARSE_BINDING),
                                     adaptorImpl->GetQueueFamilyIndex(QueueType::PRESENT)};

  auto queueFamilyProperties = physDevice.getQueueFamilyProperties();

  std::sort(newArray.begin(), newArray.end());

  mVkQueueArray.resize(newArray.size());

  int                  oldIndex = -1;
  std::vector< float > priorities;
  for(auto& newIndex : newArray)
  {
    if(newIndex != oldIndex)
    {
      auto& familyProperties = queueFamilyProperties[newIndex];

      // set priorities ( all queues have priority 1.0 to keep it simple )
      priorities.resize(familyProperties.queueCount);
      std::fill(priorities.begin(), priorities.end(), 1.0f);

      // just resize arrays
      mVkQueueArray[newIndex].resize(familyProperties.queueCount);

      vk::DeviceQueueCreateInfo queueInfo;
      queueInfo.setQueueCount(familyProperties.queueCount);
      queueInfo.setQueueFamilyIndex(newIndex);
      queueInfo.setPQueuePriorities(priorities.data());

      queueInfoArray.emplace_back(queueInfo);

      oldIndex = newIndex;
    }
  }

  vk::DeviceCreateInfo deviceInfo;
  deviceInfo.setQueueCreateInfoCount(queueInfoArray.size());
  deviceInfo.setPQueueCreateInfos(queueInfoArray.data());
  deviceInfo.setEnabledExtensionCount(mEnabledExtensionCStr.size());
  deviceInfo.setPpEnabledExtensionNames(mEnabledExtensionCStr.data());
  deviceInfo.setPEnabledFeatures(&features);

  auto result = physDevice.createDevice(deviceInfo, adaptorImpl->GetVkAllocator());
  if(result.result != vk::Result::eSuccess)
  {
    // todo rollback stuff
    VkLog("[GraphicsAdaptor] Can't create VkDevice!");
    return false;
  }

  mVkDevice = result.value;

  int familyIndex = 0;
  for(auto& family : mVkQueueArray)
  {
    for(int queueIndex = 0; queueIndex < family.size(); ++queueIndex)
    {
      mVkDevice.getQueue(familyIndex, queueIndex, &family[queueIndex]);
    }
    ++familyIndex;
  }

  VkLog("[GraphicsAdaptor] VkDevice created.");
}

vk::Device VulkanContext::GetVkDevice() const
{
  return mVkDevice;
}

vk::Queue VulkanContext::GetVkQueue(int index, QueueType type)
{
  int queueFamilyIndex = mGraphicsAdaptor.GetObjectAs< VulkanAdaptor >()->GetQueueFamilyIndex(type);
  return mVkQueueArray[queueFamilyIndex].at(index);
}

VulkanCommandPool VulkanContext::GetMainCommandPool()
{
  if( !mTransientPool ) // pool not yet initialised
  {
    mTransientPool = VulkanCommandPool::New( *this, QueueType::GRAPHICS, false, true, true );
  }
  return mTransientPool;
}

GraphicsSwapchain VulkanContext::CreateSwapchain(const GraphicsSurface& surface,
                                                 uint32_t               bufferCount,
                                                 DepthStencil           depthStencil,
                                                 bool                   enforceVSync)
{
  auto impl = new VulkanSwapchain(GraphicsContext(this), surface, bufferCount, depthStencil, enforceVSync);

  // initialise
  if(!impl->Initialise())
  {
    delete impl;
    impl = nullptr;
  }

  return GraphicsSwapchain(impl);
}

int32_t VulkanContext::GetMemoryIndex(uint32_t memoryTypeBits, vk::MemoryPropertyFlags properties)
{
  auto& memprops = mGraphicsAdaptor.GetObjectAs< VulkanAdaptor >()->GetVkMemoryProperties();
  for(int32_t i = 0; i < memprops.memoryTypeCount; ++i)
  {
    if((memoryTypeBits & (1 << i)) && ((memprops.memoryTypes[i].propertyFlags & properties) == properties))
    {
      return i;
    }
  }
  return -1;
}

vk::DeviceMemory VulkanContext::AllocateMemory(vk::Image image, vk::MemoryPropertyFlags flags, bool doBind)
{
  vk::MemoryRequirements req {};
  vk::DeviceMemory       memory { nullptr };
  mVkDevice.getImageMemoryRequirements(image, &req);

  vk::MemoryAllocateInfo info;
  info.setMemoryTypeIndex(GetMemoryIndex(req.memoryTypeBits, flags)).setAllocationSize(req.size);

  VkAssertCall( mVkDevice.allocateMemory(&info, mGraphicsAdaptor.GetObjectAs< VulkanAdaptor >()->GetVkAllocator(), &memory) );

  if( doBind )
  {
    VkAssertCall( mVkDevice.bindImageMemory( image, memory, 0 ) );
  }
  return memory;
}

vk::DeviceMemory VulkanContext::AllocateMemory(vk::Buffer buffer, vk::MemoryPropertyFlags flags, bool doBind)
{
  vk::MemoryRequirements req {};
  vk::DeviceMemory       memory { nullptr };
  mVkDevice.getBufferMemoryRequirements(buffer, &req);

  vk::MemoryAllocateInfo info;
  info.setMemoryTypeIndex(GetMemoryIndex(req.memoryTypeBits, flags)).setAllocationSize(req.size);

  VkAssertCall( mVkDevice.allocateMemory(&info, mGraphicsAdaptor.GetObjectAs< VulkanAdaptor >()->GetVkAllocator(), &memory) );

  if( doBind )
  {
    VkAssertCall( mVkDevice.bindBufferMemory( buffer, memory, 0 ) );
  }
  return memory;
}

} //

} //

} //

} //