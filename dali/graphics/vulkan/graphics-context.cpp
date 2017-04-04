//
// Created by adam.b on 31/03/17.
//

#include "graphics-context.h"
#include <dali/graphics/vulkan/graphics-adaptor.h>
#include <dali/graphics/vulkan/internal/graphics-surface-impl-base.h>
#include <dali/graphics/vulkan/internal/graphics-context-impl-base.h>
namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
class GraphicsSurface;
GraphicsSwapchain GraphicsContext::CreateSwapchain( const GraphicsSurface& surface, uint32_t bufferCount )
{
  return GetObject()->CreateSwapchain( surface, bufferCount );
}

GraphicsContext::GraphicsContext( Internal::GraphicsContextImplBase* impl )
{
  SetObject( impl );
}

GraphicsAdaptor& GraphicsContext::GetGraphicsAdaptor()
{
  return GetObject()->GetGraphicsAdaptor();
}

} // Vulkan

} // Graphics

} // Dali