//
// Created by adam.b on 03/04/17.
//

#include "graphics-swapchain.h"

#include <dali/graphics/vulkan/internal/graphics-swapchain-impl-base.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

GraphicsSwapchain::GraphicsSwapchain( Internal::GraphicsSwapchainImplBase* impl )
{
  SetObject( impl );
}

void GraphicsSwapchain::SwapBuffers( bool vsync )
{
  GetObject()->SwapBuffers( vsync );
}

}
}
}