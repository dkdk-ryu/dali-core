//
// Created by adam.b on 03/04/17.
//

#ifndef DALI_CORE_GRAPHICS_ADAPTOR_IMPL_BASE_H
#define DALI_CORE_GRAPHICS_ADAPTOR_IMPL_BASE_H

#include <dali/graphics/vulkan/graphics-adaptor.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
// abstract class for building the graphics adaptor
class GraphicsAdaptorImplBase
{
public:
  GraphicsAdaptorImplBase()                 = default;
  GraphicsAdaptorImplBase(const GraphicsAdaptorImplBase&) = default;
  GraphicsAdaptorImplBase(GraphicsAdaptorImplBase&)       = default;
  GraphicsAdaptorImplBase(GraphicsAdaptorImplBase&&)      = default;
  //GraphicsAdaptorImplBase operator=(GraphicsAdaptorImplBase&&) = delete;
  virtual ~GraphicsAdaptorImplBase()             = default;

  /**
   *
   * @param extensions
   * @param layers
   * @return
   */
  virtual bool Initialise(const ExtensionNameList& extensions, const ValidationLayerFlags2& layers) = 0;

  /**
   *
   * @param instanceExtensionName
   * @return
   */
  virtual bool IsExtensionAvailable(const std::string& instanceExtensionName) = 0;

  /**
   *
   * @param instanceExtensionName
   * @return
   */
  virtual bool IsLayerAvailable(const std::string& instanceExtensionName) = 0;

  /**
   *
   * @param surface
   * @return
   */
  virtual GraphicsContext CreateContext(const GraphicsSurface& surface) {};

  /**
   *
   * @param flags
   * @return
   */
  virtual bool ChoosePhysicalDevice(const PhysicalDeviceFlags& flags) = 0;

  /**
   *
   * @param flags
   */
  virtual void SetValidationDebugChannels(const ValidationChannelFlags &flags) = 0;
};
}
}
}
}

#endif //DALI_CORE_GRAPHICSADAPTORIMPLBASE_H
