//
// Created by adam.b on 03/04/17.
//

#include "graphics-adaptor.h"
#include <dali/graphics/vulkan/internal/graphics-adaptor-impl-base.h>
namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

GraphicsAdaptor::GraphicsAdaptor(Internal::GraphicsAdaptorImplBase* impl)
{
  SetObject( impl );
}

bool GraphicsAdaptor::Initialise(const ExtensionNameList& extensions, const ValidationLayerFlags2& layers)
{
  return GetObject()->Initialise(extensions, layers);
}

bool GraphicsAdaptor::IsExtensionAvailable(const std::string& extensionName)
{
  return GetObject()->IsExtensionAvailable(extensionName);
}

bool GraphicsAdaptor::IsLayerAvailable(const std::string& layerName)
{
  return GetObject()->IsExtensionAvailable(layerName);
}

GraphicsContext GraphicsAdaptor::CreateContext(const GraphicsSurface& surface)
{
  return GetObject()->CreateContext(surface);
}

bool GraphicsAdaptor::ChoosePhysicalDevice(const PhysicalDeviceFlags& flags)
{
  return GetObject()->ChoosePhysicalDevice(flags);
}

void GraphicsAdaptor::SetValidationDebugChannels(const ValidationChannelFlags& flags)
{
  GetObject()->SetValidationDebugChannels(flags);
}
}
}
}
