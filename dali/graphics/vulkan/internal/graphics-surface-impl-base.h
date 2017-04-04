//
// Created by adam.b on 31/03/17.
//

#ifndef DALI_CORE_GRAPHICS_SURFACE_IMPL_BASE_H
#define DALI_CORE_GRAPHICS_SURFACE_IMPL_BASE_H

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
// base class for the GraphicsSurfaceImplBase
class GraphicsAdaptor;

namespace Internal
{
class GraphicsSurfaceImplBase
{
private:
  GraphicsSurfaceImplBase() = default;

public:
  GraphicsSurfaceImplBase(const GraphicsAdaptor &adaptor){};

  virtual ~GraphicsSurfaceImplBase() = default;

  virtual bool Initialise() = 0;
  virtual bool Replace()    = 0;
  virtual bool Destroy()    = 0;
  virtual int  GetWidth()   = 0;
  virtual int  GetHeight()  = 0;
};
}
}
}
}

#endif //DALI_CORE_GRAPHICSSURFACE_H
