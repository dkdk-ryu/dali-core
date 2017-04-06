//
// Created by adam.b on 03/04/17.
//

#ifndef DALI_CORE_GRAPHICS_SWAPCHAIN_H
#define DALI_CORE_GRAPHICS_SWAPCHAIN_H

#include <cinttypes>
#include "common.h"

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

enum class DepthStencil
{
  NONE,
  DEPTH_16,
  DEPTH_24,
  DEPTH_16_STENCIL_8,
  DEPTH_24_STENCIL_8
};

namespace Internal
{
class GraphicsSwapchainImplBase;
}

class GraphicsAdaptor;
class GraphicsContext;

class GraphicsSwapchain : public Handle<Internal::GraphicsSwapchainImplBase>
{
public:

  GraphicsSwapchain() = default;
  GraphicsSwapchain(const GraphicsSwapchain&) = default;
  GraphicsSwapchain(GraphicsSwapchain&) = default;
  GraphicsSwapchain(GraphicsSwapchain&&) = default;
  GraphicsSwapchain operator=(GraphicsSwapchain&&) = delete;
  virtual ~GraphicsSwapchain() = default;

  GraphicsSwapchain( Internal::GraphicsSwapchainImplBase* impl );

  void SwapBuffers( bool vsync );

  int GetBufferCount();

  int GetCurrentBuffer();

};

}
}
}


#endif //DALI_CORE_GRAPHICSSWAPCHAIN_H
