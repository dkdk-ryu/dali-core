#ifndef DALI_GRAPHICS_VULKAN_ADAPTOR_H
#define DALI_GRAPHICS_VULKAN_ADAPTOR_H

#include <dali/graphics/vulkan/common.h>
#include <dali/graphics/vulkan/graphics-context.h>
#include <dali/graphics/vulkan/internal/graphics-surface-impl-base.h>
#include <dali/graphics/vulkan/internal/graphics-adaptor-impl-base.h>
#include <string>
#include <vector>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

namespace Internal
{
class VulkanSurface;

using ValidationLayerNameTable2 = std::vector< const char * >;

enum class QueueType : uint32_t
{
  GRAPHICS,
  COMPUTE,
  TRANSFER,
  SPARSE_BINDING,
  PRESENT,
  END
};

using VkExtensionList = std::vector< vk::ExtensionProperties >;
using VkLayerList     = std::vector< vk::LayerProperties >;

class PhysicalDeviceImpl;
// ------------------------------------------------------------------------
// todo: Move into PhysicalDevice
// this may be made common
struct PhysicalDevice
{
  PhysicalDevice(PhysicalDeviceImpl *impl) : mImpl(impl)
  {
  }

  ~PhysicalDevice()
  {
  }

  std::string mVendor{"invalid"};
  std::string mName{"null"};
  uint32_t    mVersion{0u};
  uint32_t    mApiVersion{0u};

  // just to compare physical device with invalid one
  bool operator==(const PhysicalDevice &rhs) const
  {
    return rhs.mImpl == mImpl;
  }

  // implementation dependent internals
  class Internal::PhysicalDeviceImpl *mImpl{nullptr};
};

extern const PhysicalDevice &PHYSICAL_DEVICE_INVALID;

// ------------------------------------------------------------------------
class VulkanAdaptor : public GraphicsAdaptorImplBase
{
public:
  VulkanAdaptor();

  virtual ~VulkanAdaptor();

  VulkanAdaptor(VulkanAdaptor &&) = default;

  VulkanAdaptor(const VulkanAdaptor &) = delete;

  //private:

  bool EnumerateExtensions();

  bool CreateInstance();

public:
  virtual bool Initialise(const ExtensionNameList &extensions, const ValidationLayerFlags2 &layers);

  virtual bool IsExtensionAvailable(const std::string &extensionName);

  virtual bool IsLayerAvailable(const std::string &layerName);

  // validation
  virtual void SetValidationDebugChannels(const ValidationChannelFlags &flags);

  virtual bool ChoosePhysicalDevice(const PhysicalDeviceFlags &flags);

  bool CreateLogicalDevice(const ExtensionNameList &extensions);

  virtual GraphicsContext CreateContext(const GraphicsSurface &surface);

  bool IsDeviceExtensionAvailable(const std::string &deviceExtensionName);

  /** enables extension, must happen before creating device */
  bool EnableDeviceExtension(const std::string &deviceExtensionName);

  int GetQueueFamilyIndex(QueueType type) const;

  // graphics device implementation specific stuff
  vk::Instance GetVkInstance() const;

  vk::PhysicalDevice GetVkPhysicalDevice() const;

  vk::AllocationCallbacks *GetVkAllocator() const;

  const vk::PhysicalDeviceMemoryProperties& GetVkMemoryProperties() const;

private:
  LayerNameList GetLayerNameTable(const ValidationLayerFlags2 &flags) const;

  void PrepareQueueFamilies(const GraphicsSurface& surface);

private:
  // Extensions and layers info
  VkExtensionList mInstanceExtensionProperties;
  VkExtensionList mDeviceExtensionProperties;
  VkLayerList     mInstanceLayerProperties;

  ExtensionNameList          mEnabledDeviceExtensions;
  std::vector< std::string > mEnabledDeviceExtensionsCopy;

  // Vulkan context objects
  vk::AllocationCallbacks *mAllocatorCallbacks{nullptr};
  vk::Instance             mInstance;

  // Queues - 1 queue per each type, skip sparse binding queue
  std::vector< vk::QueueFamilyProperties > mQueueFamilyProperties;

  vk::Queue mGraphicsQueue;
  vk::Queue mTransferQueue;
  vk::Queue mComputeQueue;

  std::array< int, static_cast< uint32_t >(QueueType::END) > mQueueFamilyIndex;

  UPtr< PhysicalDevice > mPhysicalDevice;

  ValidationChannelFlags   mValidationChannels{0u};
  VkDebugReportCallbackEXT mValidationReportCallback{nullptr};
};
}
}
}
}
#endif // DALI_GRAPHICS_VULKAN_CONTEXT_H
