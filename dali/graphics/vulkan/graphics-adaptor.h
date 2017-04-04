//
// Created by adam.b on 03/04/17.
//

#ifndef DALI_CORE_GRAPHICS_ADAPTOR_H
#define DALI_CORE_GRAPHICS_ADAPTOR_H

#include <dali/graphics/vulkan/common.h>
#include <dali/graphics/vulkan/graphics-context.h>
#include <dali/graphics/vulkan/graphics-surface.h>
#include <dali/graphics/vulkan/internal/graphics-surface-impl-base.h>
#include <utility>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
class GraphicsAdaptorImplBase;
}

class GraphicsAdaptor : public Handle<Internal::GraphicsAdaptorImplBase>
{
public:
  GraphicsAdaptor()                        = default;
  GraphicsAdaptor(const GraphicsAdaptor&) = default;
  GraphicsAdaptor(GraphicsAdaptor&)       = default;
  GraphicsAdaptor(GraphicsAdaptor&&)      = default;
  GraphicsAdaptor operator=(GraphicsAdaptor&&) = delete;
  ~GraphicsAdaptor()                            = default;

  GraphicsAdaptor(Internal::GraphicsAdaptorImplBase* impl);

  // creates a GraphicsAdapter based on particular implementation
  // ( this may be setup in the build time )
  // could be switched by using different namespace etc.
  template< typename T >
  static GraphicsAdaptor New()
  {
    return std::move(GraphicsAdaptor(new T()));
  }

  // public interface

  /**
   *
   * @param extensions
   * @param layers
   * @return
   */
  bool Initialise(const ExtensionNameList& extensions, const ValidationLayerFlags2& layers);

  /**
   *
   * @param instanceExtensionName
   * @return
   */
  bool IsExtensionAvailable(const std::string& instanceExtensionName);

  /**
   *
   * @param instanceExtensionName
   * @return
   */
  bool IsLayerAvailable(const std::string& instanceExtensionName);

  /**
   *
   * @param surface
   * @return
   */
  GraphicsContext CreateContext(const GraphicsSurface& surface);

  /**
   *
   * @param flags
   * @return
   */
  bool ChoosePhysicalDevice(const PhysicalDeviceFlags& flags = 0u);

  /**
   *
   * @param flags
   */
  void SetValidationDebugChannels(const ValidationChannelFlags &flags);

  /**
   *
   * @tparam T
   * @tparam Args
   * @param device
   * @param args
   * @return
   */
  template< typename T, typename... Args >
  GraphicsSurface CreateSurface(GraphicsAdaptor& adaptor, Args&&... args)
  {
    auto impl = new T(adaptor, args...);

    auto retval = GraphicsSurface( impl );
    if(!retval.Initialise())
    {
      return GraphicsSurface(nullptr);
    }
    return retval;
  };

};
}
}
}

#endif //DALI_CORE_GRAPHICSADAPTOR2_H
