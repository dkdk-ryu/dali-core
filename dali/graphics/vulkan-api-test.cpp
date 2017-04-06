//
// Created by adam.b on 23/03/17.
//

#include <vulkan/vulkan.hpp>
#ifdef __linux__
#include <X11/Xlib.h>
#include <unistd.h>
#endif


#include <dali/graphics/vulkan/graphics-adaptor.h>
// fixme: rather than using internal to provide template type
// we shoud either register available adaptors or use compile time
// deduction
#include <dali/graphics/vulkan/graphics-context.h>
#include <dali/graphics/vulkan/internal/vulkan-adaptor.h>
#include <dali/graphics/vulkan/internal/vulkan-swapchain.h>
#include <dali/graphics/vulkan/surface/graphics-xlib-surface.h>

using namespace Dali::Graphics::Vulkan;

struct TestWindow
{
  int      width, height;
  Display* display;
  ::Window window;
  int      defaultScreen;
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

int main(int argc, char** argv)
{
  InitWindow(720, 360);

  // using template argument to pick adaptor implementation
  GraphicsAdaptor   adaptor = GraphicsAdaptor::New< Internal::VulkanAdaptor >();
  ExtensionNameList list;

  // check extension
  if(adaptor.IsExtensionAvailable(VK_KHR_SURFACE_EXTENSION_NAME))
    list.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
  if(adaptor.IsExtensionAvailable(VK_KHR_XLIB_SURFACE_EXTENSION_NAME))
    list.emplace_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);

  // initialise
  adaptor.Initialise(list,
                     ValidationLayerFlags2()
  | ValidationLayerBit2::CORE_VALIDATION |
                         ValidationLayerBit2::STANDARD_VALIDATION |
                         ValidationLayerBit2::PARAMETER_VALIDATION | ValidationLayerBit2::API_DUMP);

  // reporting on all channels
  adaptor.SetValidationDebugChannels(ValidationChannelBit::ALL);

  // choose first available physical adaptor
  bool result = adaptor.ChoosePhysicalDevice(PhysicalDeviceBit::ANY);

  if(!result) // no requested device available ( maybe adding enumeration )
  {
    // fixme, something went very wrong
  }

  // create surface for X11 ( could be templated GraphicsAdaptor function? )
  auto surface = adaptor.CreateSurface< Internal::GraphicsXlibSurface >(adaptor, gWnd.display, gWnd.window);

  if(!surface)
  {
    VkLog("No surface dammit!");
  }

  // create context
  auto context = adaptor.CreateContext(surface);

  // create swapchain ( 2 buffers )
  auto swapchain = context.CreateSwapchain(surface, 2, DepthStencil::DEPTH_24, true);

  while( 1 )
  {
    usleep(16000);
    swapchain.SwapBuffers( true );
  }

  return 0;
}

int _main(int argc, char** argv)
{
#if 0
  InitWindow(720, 360);

  GraphicsAdaptor adaptor;
  ExtensionNameList list;

  // check extension
  if(adaptor.IsExtensionAvailable(VK_KHR_SURFACE_EXTENSION_NAME))
    list.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
  if(adaptor.IsExtensionAvailable(VK_KHR_XLIB_SURFACE_EXTENSION_NAME))
    list.emplace_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);

  // initialise
  adaptor.Initialise(list, ValidationLayerFlags2() |
                    ValidationLayerBit2::CORE_VALIDATION | ValidationLayerBit2::STANDARD_VALIDATION |
  ValidationLayerBit2::PARAMETER_VALIDATION|ValidationLayerBit2::API_DUMP);

  // reporting on all channels
  adaptor.SetValidationDebugChannels( ValidationChannelBit::ALL );

  // choose first available physical adaptor
  bool result = adaptor.ChoosePhysicalDevice(PhysicalDeviceBit::ANY);

  if( !result ) // no requested device available ( maybe adding enumeration )
  {
    // fixme, something went very wrong
  }

  // create surface for X11 ( could be templated GraphicsAdaptor function? )
  //auto& surface = GraphicsSurfaceImplBase::CreateSurface<GraphicsXlibSurface>( adaptor, gWnd.display, gWnd.window );
  auto& surface = adaptor.CreateSurface<GraphicsXlibSurface>( adaptor, gWnd.display, gWnd.window );

  if( surface == nullptr )
  {
    // surface invalid :((((
  }
  // create context
  GraphicsContext context = adaptor.CreateContext(surface);

  // create swapchain with 2 buffers
  GraphicsSwapchain swapchain = context.CreateSwapchain( surface, 2 );

#endif

  return 0;
}