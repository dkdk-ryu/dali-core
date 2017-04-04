//
// Created by adam.b on 03/04/17.
//

#ifndef DALI_CORE_GRAPHICS_SWAPCHAIN_H
#define DALI_CORE_GRAPHICS_SWAPCHAIN_H

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

namespace Internal
{
class GraphicsSwapchainImplBase;
}

class GraphicsAdaptor;
class GraphicsSurfaceImplBase;

class GraphicsSwapchain
{
public:

  GraphicsSwapchain() = default;
  GraphicsSwapchain(const GraphicsSwapchain&) = default;
  GraphicsSwapchain(GraphicsSwapchain&) = default;
  GraphicsSwapchain(GraphicsSwapchain&&) = default;
  GraphicsSwapchain operator=(GraphicsSwapchain&&) = delete;
  virtual ~GraphicsSwapchain() = default;

  GraphicsSwapchain( Internal::GraphicsSwapchainImplBase* impl );

  static GraphicsSwapchain New( GraphicsAdaptor& device, GraphicsSurfaceImplBase& surface );

  void SwapBuffers();

private:

  Internal::GraphicsSwapchainImplBase* mImpl = { nullptr };

};

}
}
}


#endif //DALI_CORE_GRAPHICSSWAPCHAIN_H
