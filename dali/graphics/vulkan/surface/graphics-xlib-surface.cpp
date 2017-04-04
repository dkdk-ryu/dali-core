//
// Created by adam.b on 31/03/17.
//

#include "graphics-xlib-surface.h"
#include <dali/graphics/vulkan/graphics-adaptor.h>
#include <dali/graphics/vulkan/internal/vulkan-adaptor.h>
#include <X11/Xlib.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
GraphicsXlibSurface::GraphicsXlibSurface(const GraphicsAdaptor &adaptor, Display *display, ::Window window)
: VulkanSurface(adaptor), mDisplay(display), mWindow(window)
{
}

bool GraphicsXlibSurface::Initialise()
{
  auto adaptor = mGraphicsAdaptor.GetObjectAs< Internal::VulkanAdaptor >();
  unsigned int tmp_u; // not used, but we don't want the call crash
  int tmp_i;
  Window root;
  XGetGeometry(mDisplay, mWindow, &root, &tmp_i, &tmp_i, &mSurfaceWidth, &mSurfaceHeight, &tmp_u, &tmp_u);
  vk::XlibSurfaceCreateInfoKHR info;
  info.setDpy(mDisplay).setWindow(mWindow);

  auto result = adaptor->GetVkInstance().createXlibSurfaceKHR(info);
  mSurface = result.value;

  if(!mSurface)
  {
    return false;
  }

  mFormats = adaptor->GetVkPhysicalDevice().getSurfaceFormatsKHR(mSurface).value;
  mDefaultFormat = mFormats[0];
  for(auto &format : mFormats)
  {
    if(format.format == vk::Format::eR8G8B8A8Unorm)
    {
      mDefaultFormat = format;
    }
  }

  return result.result == vk::Result::eSuccess;
}

bool GraphicsXlibSurface::Replace()
{
}

bool GraphicsXlibSurface::Destroy()
{
}

int GraphicsXlibSurface::GetWidth()
{
  return mSurfaceWidth;
}

int GraphicsXlibSurface::GetHeight()
{
  return mSurfaceHeight;
}
}
}
}
}