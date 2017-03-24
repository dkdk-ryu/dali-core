#ifndef DALI_GRAPHICS_VULKAN_SURFACE_H
#define DALI_GRAPHICS_VULKAN_SURFACE_H

#include <memory>

#ifdef __linux__
#include <X11/Xlib.h>
#endif

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

/**
 * Base class for all SurfaceInfo descriptors like
 * SurfaceXlibInfo, SurfaceXcbInfo
 */
class SurfaceInfo
{
public:
  enum class Type
  {
    XLIB,
    XCB,
    WAYLAND,
    ANDROID_NATIVE_WINDOW,
  };

  SurfaceInfo( Type type ) : mType( type ) {}
  virtual ~SurfaceInfo() = default;


  Type mType;
};

#ifdef __linux__

class SurfaceXlibInfo : public SurfaceInfo
{
public:
  SurfaceXlibInfo(Display* display, Window window) : SurfaceInfo(Type::XLIB)
  {
  }
  virtual ~SurfaceXlibInfo() = default;

  // X11 data
  Display* mDisplay{ nullptr };
  Window   mWindow;
};

class SurfaceXcbInfo : public SurfaceInfo
{
public:
  SurfaceXcbInfo() : SurfaceInfo(Type::XCB)
  {
  }
  virtual ~SurfaceXcbInfo() = default;
};

#endif

class Window;
struct SurfaceImpl;
class Surface
{
public:
  Surface()                = default;
  virtual ~Surface()       = default;
  Surface(const Surface&&) = delete;
  Surface(const Surface&)  = delete;

  /**
   * Create surface from given Window
   * @param window
   * @return
   */
  static Surface New(Window* window);

private:
  std::unique_ptr< SurfaceImpl > mImpl;
};
}
}
}

#endif // SURFACE_H
