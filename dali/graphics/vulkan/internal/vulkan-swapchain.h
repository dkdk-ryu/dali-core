//
// Created by adam.b on 03/04/17.
//

#ifndef DALI_CORE_VULKAN_SWAPCHAIN_H
#define DALI_CORE_VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>

#include "graphics-swapchain-impl-base.h"

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
class GraphicsContext;
class GraphicsSurface;

namespace Internal
{

class VulkanSwapchain : public GraphicsSwapchainImplBase
{
public:
  VulkanSwapchain( const GraphicsContext& context, const GraphicsSurface& surface );

  virtual bool Initialise();

  vk::SwapchainKHR GetVkSwapchain() const;

private:

  bool CreateBuffers();

private:

  vk::SwapchainKHR mSwapchain;

};

}
}
}
}


#endif //DALI_CORE_VULKANSWAPCHAIN_H
