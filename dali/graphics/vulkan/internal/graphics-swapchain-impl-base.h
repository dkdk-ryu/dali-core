//
// Created by adam.b on 03/04/17.
//

#ifndef DALI_CORE_GRAPHICS_SWAPCHAIN_IMPL_H
#define DALI_CORE_GRAPHICS_SWAPCHAIN_IMPL_H

#include <dali/graphics/vulkan/graphics-context.h>
#include <dali/graphics/vulkan/graphics-surface.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{

class GraphicsSwapchainImplBase
{
public:
  GraphicsSwapchainImplBase( const GraphicsContext& context, const GraphicsSurface& surface );
  virtual ~GraphicsSwapchainImplBase() = default;

  virtual bool Initialise() = 0;

  virtual void SwapBuffers( bool vsync ) = 0;

protected:

  GraphicsContext mContext;
  GraphicsSurface mSurface;
};

} // Internal
} // Vulkan
} // Graphics
} // Dali


#endif //DALI_CORE_GRAPHICSSWAPCHAINIMPL_H
