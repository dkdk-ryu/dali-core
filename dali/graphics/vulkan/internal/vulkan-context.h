//
// Created by adam.b on 31/03/17.
//

#ifndef DALI_CORE_VULKAN_CONTEXT_H
#define DALI_CORE_VULKAN_CONTEXT_H

#include <dali/graphics/vulkan/common.h>
#include <dali/graphics/vulkan/internal/graphics-context-impl-base.h>
#include <dali/graphics/vulkan/graphics-swapchain.h>
#include <vulkan/vulkan.hpp>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
class GraphicsAdaptor;
class GraphicsSurface;

namespace Internal
{

class VulkanContext : public Internal::GraphicsContextImplBase
{
public:
  VulkanContext(const GraphicsAdaptor& adaptor, const GraphicsSurface& surface, const ExtensionNameList& extensions);
  virtual ~VulkanContext() = default;

  bool Initialise();

  virtual GraphicsSwapchain CreateSwapchain( const GraphicsSurface& surface, uint32_t bufferCount );

  vk::Device GetVkDevice() const;

private:

  vk::Device mVkDevice { nullptr };

  ExtensionNameList          mEnabledExtensionCStr;
  std::vector< std::string > mEnabledExtensionStr;
};

} // Internal
} // Vulkan
} // Graphics
} // Dali

#endif //DALI_CORE_GRAPHICSCONTEXTIMPL_H
