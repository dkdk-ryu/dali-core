//
// Created by adam.b on 31/03/17.
//

#include <dali/graphics/vulkan/internal/vulkan-context.h>
#include <dali/graphics/vulkan/graphics-adaptor.h>
#include <dali/graphics/vulkan/internal/graphics-surface-impl-base.h>
#include <dali/graphics/vulkan/internal/vulkan-adaptor.h>
#include <dali/graphics/vulkan/surface/vulkan-surface.h>
#include <dali/graphics/vulkan/internal/vulkan-swapchain.h>

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
: GraphicsContextImplBase( adaptor, surface, extensions )
{
  for(auto& name : extensions)
  {
    mEnabledExtensionStr.emplace_back(name);
    mEnabledExtensionCStr.emplace_back(mEnabledExtensionStr.back().c_str());
  }
}

bool VulkanContext::Initialise()
{
  auto adaptorImpl = mGraphicsAdaptor.GetObjectAs<VulkanAdaptor>();
  auto physDevice = adaptorImpl->GetVkPhysicalDevice();
  auto features   = physDevice.getFeatures();
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

  VkLog("[GraphicsAdaptor] VkDevice created.");
}

vk::Device VulkanContext::GetVkDevice() const
{
  return mVkDevice;
}

GraphicsSwapchain VulkanContext::CreateSwapchain( const GraphicsSurface& surface, uint32_t bufferCount )
{
  GraphicsContext context(this);
  VulkanSwapchain* impl = new VulkanSwapchain( context, surface );
  if( !impl->Initialise() )
  {
    delete impl;
    return GraphicsSwapchain(); // nullptr
  }

  return GraphicsSwapchain( impl );
}

} //

} //

} //

} //