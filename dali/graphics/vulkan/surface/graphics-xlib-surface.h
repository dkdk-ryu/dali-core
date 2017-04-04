//
// Created by adam.b on 31/03/17.
//

#ifndef DALI_CORE_GRAPHICS_XLIB_SURFACE_H
#define DALI_CORE_GRAPHICS_XLIB_SURFACE_H

#include <dali/graphics/vulkan/surface/vulkan-surface.h>

#include <X11/Xlib.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
class GraphicsXlibSurface : public VulkanSurface
{
public:
  GraphicsXlibSurface(const GraphicsAdaptor &adaptor, Display *display, Window window);

  ~GraphicsXlibSurface() = default;

  virtual bool Initialise();

  virtual bool Replace();

  virtual bool Destroy();

  virtual int GetWidth();

  virtual int GetHeight();

protected:
  // X11
  ::Display *mDisplay;
  ::Window mWindow;
};
}
}
}
}
#endif //DALI_CORE_GRAPHICSXLIBSURFACE_H
