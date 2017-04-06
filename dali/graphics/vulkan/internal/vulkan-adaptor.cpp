#include "vulkan-adaptor.h"
#include <dali/graphics/vulkan/surface/vulkan-surface.h>
#include <dali/graphics/vulkan/internal/vulkan-context.h>

#include <cstdarg>
namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
#define MAX_VKLOG_SIZE (1024 * 1024) // 1mb of log memory per thread
__thread char gLogBuffer[MAX_VKLOG_SIZE];

// something to replace in the future, maybe
void VkLog(const char* format, ...)
{
  std::va_list args;
  va_start(args, format);
  vsnprintf(gLogBuffer, MAX_VKLOG_SIZE - 4, format, args);
  puts(gLogBuffer);
  va_end(args);
}

namespace Internal
{
// Vulkan specific data, all public
struct PhysicalDeviceImpl
{
  vk::PhysicalDevice                 gpu{nullptr};
  vk::PhysicalDeviceFeatures         features{};
  vk::PhysicalDeviceProperties       properties{};
  vk::PhysicalDeviceMemoryProperties memoryProperties{};
  vk::PhysicalDeviceType             type;
  vk::PhysicalDeviceLimits           limits;
  bool                               isDiscrete{false};
};

namespace
{

template< typename T >
inline int Eval(const T& type)
{
  return static_cast< int >(type);
}


// debug callbacks
VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(VkDebugReportFlagsEXT      flags,
                                                   VkDebugReportObjectTypeEXT objectType,
                                                   uint64_t                   object,
                                                   size_t                     location,
                                                   int32_t                    messageCode,
                                                   const char*                pLayerPrefix,
                                                   const char*                pMessage,
                                                   void*                      pUserData)
{
  VkLog("[VALIDATION:%s]: %s", pLayerPrefix, pMessage);
  return VK_FALSE;
}

const char* VALIDATION_LAYERS[] = {"VK_LAYER_LUNARG_screenshot",           // screenshot
                                   "VK_LAYER_LUNARG_parameter_validation", // parameter
                                   "VK_LAYER_LUNARG_vktrace", // vktrace ( requires vktrace connection )
                                   "VK_LAYER_LUNARG_monitor",             // monitor
                                   "VK_LAYER_LUNARG_swapchain",           // swapchain
                                   "VK_LAYER_GOOGLE_threading",           // threading
                                   "VK_LAYER_LUNARG_api_dump",            // api
                                   "VK_LAYER_LUNARG_object_tracker",      // objects
                                   "VK_LAYER_LUNARG_core_validation",     // core
                                   "VK_LAYER_GOOGLE_unique_objects",      // unique objects
                                   "VK_LAYER_LUNARG_standard_validation", // standard
                                   nullptr};

static PhysicalDevice nulldevice{nullptr};
}



const PhysicalDevice& PHYSICAL_DEVICE_INVALID(nulldevice);

VulkanAdaptor::VulkanAdaptor()
{
}

VulkanAdaptor::~VulkanAdaptor() = default;

bool VulkanAdaptor::IsExtensionAvailable(const std::string& instanceExtensionName)
{
  if(mInstanceExtensionProperties.empty())
  {
    auto result = vk::enumerateInstanceExtensionProperties();
    if(result.result != vk::Result::eSuccess)
    {
      // todo no vulkan support????
      return false;
    }
    mInstanceExtensionProperties = std::move(result.value);
  }

  for(auto& ext : mInstanceExtensionProperties)
  {
    if(instanceExtensionName == ext.extensionName)
    {
      return true;
    }
  }
  return false;
}

bool VulkanAdaptor::IsLayerAvailable(const std::string& instanceLayerName)
{
  if(mInstanceLayerProperties.empty())
  {
    auto result = vk::enumerateInstanceLayerProperties();
    if(result.result != vk::Result::eSuccess)
    {
      // todo no vulkan support????
      return false;
    }
    mInstanceLayerProperties = std::move(result.value);
  }

  for(auto& ext : mInstanceLayerProperties)
  {
    if(instanceLayerName == ext.layerName)
    {
      return true;
    }
  }
  return false;
}

bool VulkanAdaptor::IsDeviceExtensionAvailable(const std::string& deviceExtensionName)
{
  if(mDeviceExtensionProperties.empty())
  {
    auto result = mPhysicalDevice->mImpl->gpu.enumerateDeviceExtensionProperties();
    if(result.result != vk::Result::eSuccess)
    {
      // todo no vulkan support????
      return false;
    }
    mDeviceExtensionProperties = std::move(result.value);
  }

  for(auto& ext : mDeviceExtensionProperties)
  {
    if(deviceExtensionName == ext.extensionName)
    {
      return true;
    }
  }
  return false;
}

bool VulkanAdaptor::Initialise(const ExtensionNameList& extensions, const ValidationLayerFlags2& layers)
{
  // make a copy
  auto extCopy = ExtensionNameList{extensions};
  if(layers != 0u)
  {
    extCopy.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }

  auto layerTable = GetLayerNameTable(layers);

  vk::InstanceCreateInfo info;
  info.setEnabledExtensionCount(extCopy.size())
      .setPpEnabledExtensionNames(extCopy.data())
      .setPApplicationInfo(nullptr)
      .setPpEnabledLayerNames(layerTable.empty() ? nullptr : layerTable.data())
      .setEnabledLayerCount(layerTable.size());
  auto result = vk::createInstance(info, mAllocatorCallbacks);
  if(result.result == vk::Result::eSuccess)
  {
    mInstance = result.value;
  }
  return result.result == vk::Result::eSuccess;
}

LayerNameList VulkanAdaptor::GetLayerNameTable(const ValidationLayerFlags2& flags) const
{
  auto retval = LayerNameList{};
  for(uint32_t bit = 0; VALIDATION_LAYERS[bit]; ++bit)
  {
    if(flags & (1 << bit))
    {
      retval.emplace_back(VALIDATION_LAYERS[bit]);
    }
  }
  return retval;
}

bool VulkanAdaptor::ChoosePhysicalDevice(const PhysicalDeviceFlags& flags)
{
  bool                         discrete{flags & PhysicalDeviceBit::DISCRETE ? true : false};
  const vk::PhysicalDeviceType preferredType =
  discrete ? vk::PhysicalDeviceType::eDiscreteGpu : vk::PhysicalDeviceType::eIntegratedGpu;

  const vk::PhysicalDeviceType fallbackType =
  discrete ? vk::PhysicalDeviceType::eIntegratedGpu : vk::PhysicalDeviceType::eDiscreteGpu;

  auto result = mInstance.enumeratePhysicalDevices();
  if(result.result != vk::Result::eSuccess)
  {
    // error, possibly no devices
    return false;
  }

  auto GPUs            = result.value;
  auto currentGPU      = GPUs.front();
  auto currentGPUProps = currentGPU.getProperties();

  // select discreet over integrated
  for(auto& gpu : GPUs)
  {
    auto gpuProperties = gpu.getProperties();
    if((currentGPUProps.deviceType != preferredType && gpuProperties.deviceType == preferredType) ||
       (currentGPUProps.deviceType != preferredType && gpuProperties.deviceType == fallbackType))
    {
      currentGPU      = gpu;
      currentGPUProps = gpuProperties;
    }
  }

  // make sure selected GPU is THE REAL GPU not a blanket or refrigerator
  if(currentGPUProps.deviceType != preferredType && currentGPUProps.deviceType != fallbackType)
  {
    // error
    return false;
  }

  // store GPU data
  // fixme
  auto& gpu                 = currentGPU;
  auto& gpuProperties       = currentGPUProps;
  auto  gpuMemoryProperties = currentGPU.getMemoryProperties();
  auto  gpuFeatures         = currentGPU.getFeatures();

  auto deviceExtensionProperties = gpu.enumerateDeviceExtensionProperties();
  mDeviceExtensionProperties     = deviceExtensionProperties.value;

  auto queueFamilyProperties = gpu.getQueueFamilyProperties();

  mPhysicalDevice = UPtr< PhysicalDevice >(new PhysicalDevice{new PhysicalDeviceImpl{}});

  auto impl              = mPhysicalDevice->mImpl;
  impl->isDiscrete       = discrete;
  impl->gpu              = gpu;
  impl->properties       = gpuProperties;
  impl->features         = gpuFeatures;
  impl->memoryProperties = gpuMemoryProperties;
  impl->limits           = impl->properties.limits;
  impl->type             = impl->properties.deviceType;

  // fill common stuff
  mPhysicalDevice->mName = impl->properties.deviceName;

  switch(impl->properties.vendorID)
  {
    case 0x10DE: //nvidia
      mPhysicalDevice->mVendor = "NVIDIA";
      break;
    default:
      mPhysicalDevice->mVendor = "Unknown";
  }

  mPhysicalDevice->mApiVersion = impl->properties.apiVersion;
  mPhysicalDevice->mVersion    = impl->properties.driverVersion;

  return mPhysicalDevice.get() != nullptr;
}

/*
bool GraphicsAdaptor::CreateGraphicsDevice(const ExtensionNameList& extensions)
{
  return true;
}
*/

bool VulkanAdaptor::EnableDeviceExtension(const std::string& deviceExtensionName)
{
  if(mDeviceExtensionProperties.empty())
  {
    return false;
  }

  if(IsDeviceExtensionAvailable(deviceExtensionName))
  {
    mEnabledDeviceExtensionsCopy.emplace_back(deviceExtensionName);
    mEnabledDeviceExtensions.emplace_back(mEnabledDeviceExtensionsCopy.back().c_str());
    return true;
  }
  return false;
}

void VulkanAdaptor::PrepareQueueFamilies(const GraphicsSurface& surface)
{
  auto vulkanSurface = static_cast<VulkanSurface*>( surface.GetObject() );
  int& familyGraphics{mQueueFamilyIndex[Eval(QueueType::GRAPHICS)]};
  int& familyTransfer{mQueueFamilyIndex[Eval(QueueType::TRANSFER)]};
  int& familyCompute{mQueueFamilyIndex[Eval(QueueType::COMPUTE)]};
  int& familySparseBinding{mQueueFamilyIndex[Eval(QueueType::SPARSE_BINDING)]};
  int& familyPresent{mQueueFamilyIndex[Eval(QueueType::PRESENT)]};

  familyGraphics = familyTransfer = familyCompute = familySparseBinding = familyPresent = -1;

  auto& gpu              = mPhysicalDevice->mImpl;
  mQueueFamilyProperties = gpu->gpu.getQueueFamilyProperties();

  int queueIndex = 0;
  for(auto& queueFamily : mQueueFamilyProperties)
  {
    if(familyGraphics < 0 && (queueFamily.queueFlags | vk::QueueFlagBits::eGraphics))
    {
      familyGraphics = queueIndex;
    }
    if(familyTransfer < 0 && (queueFamily.queueFlags | vk::QueueFlagBits::eTransfer))
    {
      familyTransfer = queueIndex;
    }
    if(familyCompute < 0 && (queueFamily.queueFlags | vk::QueueFlagBits::eCompute))
    {
      familyCompute = queueIndex;
    }
    if(familySparseBinding < 0 && (queueFamily.queueFlags | vk::QueueFlagBits::eSparseBinding))
    {
      familySparseBinding = queueIndex;
    }

    if(familyPresent < 0)
    {
      vk::Bool32 supported{false};

      mPhysicalDevice->mImpl->gpu.getSurfaceSupportKHR(queueIndex, vulkanSurface->GetVkSurface(), &supported);
      if(supported)
      {
        familyPresent = queueIndex;
      }
    }

    ++queueIndex;
  }
}

vk::Instance VulkanAdaptor::GetVkInstance() const
{
  return mInstance;
}

GraphicsContext VulkanAdaptor::CreateContext(const GraphicsSurface& _surface)
{
  auto surface = dynamic_cast<VulkanSurface*>(_surface.GetObject());
  if(surface == nullptr)
  {
    return GraphicsContext();
  }

  // must be a swapchain extension!
  if(!EnableDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
  {
    // fixme no swapchain ( old S7 days )
    return GraphicsContext();
  }

  // must be surface created till now, so surface object may be passed
  PrepareQueueFamilies(_surface);

  return GraphicsContext::New<VulkanContext>( GraphicsAdaptor(this), _surface, mEnabledDeviceExtensions );
}

int VulkanAdaptor::GetQueueFamilyIndex( QueueType type ) const
{
  return mQueueFamilyIndex[Eval(type)];
}

vk::PhysicalDevice VulkanAdaptor::GetVkPhysicalDevice() const
{
  // bit workaround
  return mPhysicalDevice->mImpl->gpu;
}

vk::AllocationCallbacks* VulkanAdaptor::GetVkAllocator() const
{
  return mAllocatorCallbacks;
}

const vk::PhysicalDeviceMemoryProperties& VulkanAdaptor::GetVkMemoryProperties() const
{
  return mPhysicalDevice->mImpl->memoryProperties;
}

void VulkanAdaptor::SetValidationDebugChannels(const ValidationChannelFlags& channelFlags)
{
  bool needsUpdate{false};
  if(channelFlags != mValidationChannels)
  {
    mValidationChannels = channelFlags;
    needsUpdate         = true;
  }

  if(!needsUpdate)
  {
    return;
  }

  // destroying callback should be synchronized ( probably )
  auto fn = reinterpret_cast< PFN_vkCreateDebugReportCallbackEXT >(
  mInstance.getProcAddr("vkCreateDebugReportCallbackEXT"));
  if(!fn)
  {
    return;
  }

  if(mValidationReportCallback)
  {
    auto fnDestroy = reinterpret_cast< PFN_vkDestroyDebugReportCallbackEXT >(
    mInstance.getProcAddr("vkDestroyDebugReportCallbackEXT"));

    fnDestroy(mInstance, mValidationReportCallback,
              reinterpret_cast< VkAllocationCallbacks* >(mAllocatorCallbacks));
  }

  if(mValidationChannels == 0u)
  {
    return;
  }

  VkDebugReportFlagsEXT                flags{0};
  vk::DebugReportCallbackCreateInfoEXT info{};

  if(mValidationChannels & ValidationChannelBit::WARNING)
  {
    flags |= VK_DEBUG_REPORT_WARNING_BIT_EXT;
  }
  if(mValidationChannels & ValidationChannelBit::DEBUG)
  {
    flags |= VK_DEBUG_REPORT_DEBUG_BIT_EXT;
  }
  if(mValidationChannels & ValidationChannelBit::ERROR)
  {
    flags |= VK_DEBUG_REPORT_ERROR_BIT_EXT;
  }
  if(mValidationChannels & ValidationChannelBit::INFORMATION)
  {
    flags |= VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
  }
  if(mValidationChannels & ValidationChannelBit::PERFORMANCE_WARNING)
  {
    flags |= VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
  }

  info.setFlags(vk::DebugReportFlagBitsEXT(flags));
  info.setPfnCallback(DebugReportCallback);
  info.setPUserData(nullptr);

  // create new report callback
  if(fn(mInstance,
        reinterpret_cast< VkDebugReportCallbackCreateInfoEXT* >(&info),
        reinterpret_cast< VkAllocationCallbacks* >(mAllocatorCallbacks),
        &mValidationReportCallback) != VK_SUCCESS)
  {
    // todo error
  }
}
}
}
}
}