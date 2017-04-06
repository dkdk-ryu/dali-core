//
// Created by adam.b on 03/04/17.
//

#ifndef DALI_CORE_GRAPHICS_CONTEXT_IMPL_BASE_H
#define DALI_CORE_GRAPHICS_CONTEXT_IMPL_BASE_H

#include <dali/graphics/vulkan/common.h>
#include <dali/graphics/vulkan/graphics-adaptor.h>
#include <dali/graphics/vulkan/graphics-context.h>
#include <dali/graphics/vulkan/graphics-surface.h>
#include <dali/graphics/vulkan/graphics-swapchain.h>
#include <cinttypes>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
// base class for context implementation
class GraphicsContextImplBase
{
public:
  GraphicsContextImplBase(const GraphicsAdaptor&   adaptor,
                          const GraphicsSurface&   surface,
                          const ExtensionNameList& extensions);
  ~GraphicsContextImplBase() = default;

  virtual bool Initialise() = 0;

  virtual GraphicsSwapchain CreateSwapchain(const GraphicsSurface& surface,
                                            uint32_t               bufferCount,
                                            DepthStencil           depthStencil,
                                            bool                   enforceVSync) = 0;

  GraphicsAdaptor& GetGraphicsAdaptor();
  GraphicsSurface& GetSurface();

protected:
  GraphicsAdaptor mGraphicsAdaptor;
  GraphicsSurface mSurface;
};

} // Internal
/*
inline Internal::GraphicsContextImplBase* GraphicsContext::GetImplementation() const
{

}*/

} // Vulkan
} // Graphics
} // Dali

#endif //DALI_CORE_GRAPHICSCONTEXTIMPLBASE_H
