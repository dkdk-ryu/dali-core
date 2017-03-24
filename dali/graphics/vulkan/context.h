#ifndef DALI_GRAPHICS_VULKAN_CONTEXT_H
#define DALI_GRAPHICS_VULKAN_CONTEXT_H


#include <vulkan/vulkan.hpp>
#include <unordered_map>
#include <vector>
#include <string>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
class Context
{
public:

  Context();
  ~Context() = default;

  Context( Context&& ) = default;
  Context( const Context& ) = delete;

//private:

  bool EnumerateExtensions();
  bool CreateInstance();

  bool QueryInstanceExtension( const std::string& instanceExtensionName );
  bool QueryInstanceLayer( const std::string& instanceLayerName );
  bool QueryDeviceExtension( const std::string& deviceExtentionName );

private:
    
  // Extensions and layers info  
  std::vector< vk::ExtensionProperties > mInstanceExtensionProperties;
  std::vector< vk::LayerProperties > mInstanceLayerProperties;

  // Vulkan context objects
  vk::Instance mInstance;
  vk::Device mDevice;

  // Queues - 1 queue per each type, skip sparse binidng queue
  vk::Queue mGraphicsQueue;
  vk::Queue mTransferQueue;
  vk::Queue mComputeQueue;

  // GPU
  vk::PhysicalDevice mGPU;
  vk::PhysicalDeviceFeatures mGPUFeatures;
  vk::PhysicalDeviceFeatures mGPUProperties;
};
}
}
}

#endif // DALI_GRAPHICS_VULKAN_CONTEXT_H
