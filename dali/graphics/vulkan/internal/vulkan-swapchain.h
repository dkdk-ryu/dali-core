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
using ImageData = std::tuple< vk::Image, vk::ImageView, vk::DeviceMemory >;
class VulkanSwapchain : public GraphicsSwapchainImplBase
{
public:
  VulkanSwapchain(const GraphicsContext& context, const GraphicsSurface& surface,
                  uint32_t bufferCount, DepthStencil depthStencilMode, bool enforceVSync);

  virtual bool Initialise();

  virtual void SwapBuffers( bool vsync );

  vk::SwapchainKHR GetVkSwapchain() const;

private:
  bool      CreateFramebuffers();
  ImageData CreateDepthStencil();

private:
  vk::SwapchainKHR mSwapchain;

  struct SwapchainImage
  {
    // image
    vk::Image            image;
    vk::ImageView        imageView;
    vk::ImageAspectFlags aspectFlags;
    vk::ImageLayout      layout;
    vk::Fence            fence;

    // depth/stencil
    bool depthStencilSharedMode{true}; // false not supported

    // framebuffer/renderpass
    vk::Framebuffer framebuffer;
    vk::RenderPass  renderpass;
  };

  struct
  {
    vk::Image        image;
    vk::ImageView    imageView;
    vk::DeviceMemory memory;
  } mDepthStencil;

  DepthStencil                  mDepthStencilMode{DepthStencil::NONE};
  bool                          mEnforceVSync{false};
  uint32_t                      mMinBufferCount{2};
  std::vector< SwapchainImage > mImages;
  uint32_t                      mCurrentImage{-1u};
};
}
}
}
}

#endif //DALI_CORE_VULKANSWAPCHAIN_H
