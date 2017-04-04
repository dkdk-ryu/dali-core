//
// Created by adam.b on 04/04/17.
//

#ifndef DALI_CORE_GRAPHICS_SURFACE_H
#define DALI_CORE_GRAPHICS_SURFACE_H

#include "common.h"
#include <dali/graphics/vulkan/internal/graphics-surface-impl-base.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

class GraphicsAdaptor;

namespace Internal
{
class GraphicsSurfaceImplBase;
}

class  GraphicsSurface : public Handle< Internal::GraphicsSurfaceImplBase >
{
public:

  GraphicsSurface( Internal::GraphicsSurfaceImplBase* impl );

  template< typename T >
  static GraphicsSurface New(const GraphicsAdaptor& adaptor)
  {
    return GraphicsSurface(new T(adaptor));
  }

  bool Initialise();
  bool Replace();
  bool Destroy();
  int  GetWidth();
  int  GetHeight();

private:
};
}
}
}

#endif //DALI_CORE_GRRAPHICSSURFACE_H
