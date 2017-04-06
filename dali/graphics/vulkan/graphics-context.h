//
// Created by adam.b on 31/03/17.
//

#ifndef DALI_CORE_GRAPHICS_CONTEXT_H
#define DALI_CORE_GRAPHICS_CONTEXT_H

#include <dali/graphics/vulkan/common.h>
#include <dali/graphics/vulkan/graphics-swapchain.h>
#include <memory>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
class GraphicsContextImplBase;
}

class GraphicsSurface;
class GraphicsAdaptor;

// context is equivalent of device
class GraphicsContext : public Handle< Internal::GraphicsContextImplBase >
{
public:
  GraphicsContext() = default;
  /*
  GraphicsContext(const GraphicsContext&) = default;
  GraphicsContext(GraphicsContext&)       = default;
  GraphicsContext(GraphicsContext&&)      = default;
  GraphicsContext operator=(GraphicsContext&&) = delete;
  ~GraphicsContext()                           = default;
*/
  GraphicsContext(Internal::GraphicsContextImplBase* impl);

  GraphicsSwapchain CreateSwapchain(const GraphicsSurface& surface,
                                    uint32_t               bufferCount,
                                    DepthStencil           depthStencilMode,
                                    bool                   enforceVSync);

public:
  // API
  template< typename T >
  static GraphicsContext New(const GraphicsAdaptor& adaptor, const GraphicsSurface& surface, const ExtensionNameList& extensions)
  {
    auto impl = new T(adaptor, surface, extensions);
    if(impl->Initialise())
    {
      return std::move(GraphicsContext(impl));
    }
    return nullptr;
  }

  GraphicsAdaptor& GetGraphicsAdaptor();

private:
};

} // Vulkan

} // Graphics

} // Dali

#endif //DALI_CORE_GRAPHICSCONTEXT_H
