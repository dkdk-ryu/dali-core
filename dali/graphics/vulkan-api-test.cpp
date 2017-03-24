//
// Created by adam.b on 23/03/17.
//

#include "vulkan/instance.h"
#include <vulkan/vulkan.hpp>
#ifdef __linux__
#include <X11/Xlib.h>
#endif

using namespace Dali::Graphics::Vulkan;

struct TestWindow
{
  int width, height;
  Display* display;
  ::Window window;
  int defaultScreen;
} gWnd;


void InitWindow(int width, int height)
{

  // 1. Create Window ( done by DALI
  gWnd.width         = width;
  gWnd.height        = height;
  gWnd.display       = XOpenDisplay(nullptr);
  gWnd.defaultScreen = DefaultScreen(gWnd.display);
  gWnd.window = XCreateSimpleWindow(gWnd.display, RootWindow(gWnd.display, gWnd.defaultScreen), 0, 0,
                                    gWnd.width, gWnd.height, 1, BlackPixel(gWnd.display, gWnd.defaultScreen),
                                   WhitePixel(gWnd.display, gWnd.defaultScreen));

  XSelectInput(gWnd.display, gWnd.window, ExposureMask | KeyPressMask);
  XMapWindow(gWnd.display, gWnd.window);
}

void Test()
{
  // get extensions
  ExtensionNameList list;
  if( Instance::IsExtensionAvailable( VK_KHR_SURFACE_EXTENSION_NAME ) )
    list.emplace_back( VK_KHR_SURFACE_EXTENSION_NAME );
  if ( Instance::IsExtensionAvailable( VK_KHR_XLIB_SURFACE_EXTENSION_NAME ) )
    list.emplace_back( VK_KHR_XLIB_SURFACE_EXTENSION_NAME );

  // create vulkan instance including validation layers
  InstancePtr instance( Instance::New( list, ValidationBit::ALL ) );

  // create surface based on given descriptor class
  instance->CreateSurface( SurfaceXlibInfo( gWnd.display, gWnd.window ) );




};
