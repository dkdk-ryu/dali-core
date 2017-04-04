//
// Created by adam.b on 31/03/17.
//

#include "vulkan-surface.h"

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
VulkanSurface::VulkanSurface(const GraphicsAdaptor &adaptor)
: GraphicsSurfaceImplBase {adaptor}, mGraphicsAdaptor(adaptor)
{

}

vk::SurfaceKHR VulkanSurface::GetVkSurface() const
{
  return mSurface;
}

GraphicsAdaptor VulkanSurface::GetGraphicsAdaptor() const
{
  return mGraphicsAdaptor;
}

vk::SurfaceFormatKHR VulkanSurface::GetVkSurfaceFormat() const
{
  return mDefaultFormat;
}

} // Internal

} // Vulkan

} // Graphics

} // Dali