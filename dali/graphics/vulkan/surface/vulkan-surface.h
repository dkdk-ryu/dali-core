//
// Created by adam.b on 31/03/17.
//

#ifndef DALI_CORE_VULKAN_SURFACE_H
#define DALI_CORE_VULKAN_SURFACE_H

#include <dali/graphics/vulkan/graphics-adaptor.h>
#include <dali/graphics/vulkan/internal/graphics-surface-impl-base.h>
#include <vulkan/vulkan.hpp>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
class VulkanSurface : public GraphicsSurfaceImplBase
{
public:
  VulkanSurface(const GraphicsAdaptor &device);

  vk::SurfaceKHR GetVkSurface() const;

  vk::SurfaceFormatKHR GetVkSurfaceFormat() const;

  GraphicsAdaptor GetGraphicsAdaptor() const;

protected:
  vk::SurfaceKHR mSurface;

  std::vector< vk::SurfaceFormatKHR > mFormats;
  vk::SurfaceFormatKHR                mDefaultFormat;

  GraphicsAdaptor mGraphicsAdaptor;

  uint32_t mSurfaceWidth{0};
  uint32_t mSurfaceHeight{0};
};

} // Internal

} // Vulkan

} // Graphics

} // Dali

#endif //DALI_CORE_VULKANSURFACE_H
