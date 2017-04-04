//
// Created by adam.b on 04/04/17.
//

#include "graphics-surface.h"
#include <dali/graphics/vulkan/internal/graphics-surface-impl-base.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

GraphicsSurface::GraphicsSurface(Internal::GraphicsSurfaceImplBase* impl)
{
  SetObject(impl);
}

bool GraphicsSurface::Initialise()
{
  return GetObject()->Initialise();
}

bool GraphicsSurface::Replace()
{
  return GetObject()->Replace();
}

bool GraphicsSurface::Destroy()
{
  return GetObject()->Destroy();
}

int GraphicsSurface::GetWidth()
{
  return GetObject()->GetWidth();
}

int GraphicsSurface::GetHeight()
{
  return GetObject()->GetHeight();
}
}
}
}