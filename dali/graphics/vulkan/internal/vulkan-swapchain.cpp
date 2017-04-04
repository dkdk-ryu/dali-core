//
// Created by adam.b on 03/04/17.
//

#include "vulkan-swapchain.h"
#include <dali/graphics/vulkan/common.h>
#include <dali/graphics/vulkan/graphics-context.h>
#include <dali/graphics/vulkan/internal/vulkan-adaptor.h>
#include <dali/graphics/vulkan/internal/vulkan-context.h>
#include <dali/graphics/vulkan/surface/vulkan-surface.h>

#include <vulkan/vulkan.hpp>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
class GraphicsAdaptor;
class GraphicsSurfaceImplBase;

namespace Internal
{

VulkanSwapchain::VulkanSwapchain(const GraphicsContext& context, const GraphicsSurface& surface)
: GraphicsSwapchainImplBase(context, surface)
{
}

bool VulkanSwapchain::Initialise()
{
  auto adaptor = mContext.GetGraphicsAdaptor().GetObjectAs< VulkanAdaptor >();
  // fixme: turn device into VulkanHandle for future requirement
  auto context = mContext.GetObjectAs< VulkanContext >();

  auto surface = mSurface.GetObjectAs< VulkanSurface >();

  auto vkInstance = adaptor->GetVkInstance();
  auto vkDevice = context->GetVkDevice();
  // --------------------------------------------------------
  vk::SwapchainCreateInfoKHR info;
  info.setClipped(true)
      .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
      .setImageArrayLayers(1)
      .setImageColorSpace(surface->GetVkSurfaceFormat().colorSpace)
      .setImageExtent({( uint32_t )surface->GetWidth(), ( uint32_t )surface->GetHeight()})
      .setImageFormat(surface->GetVkSurfaceFormat().format)
      .setImageSharingMode(vk::SharingMode::eExclusive)
      .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
      .setMinImageCount(2)
      .setOldSwapchain(nullptr)
      .setPQueueFamilyIndices(nullptr)
      .setPresentMode(vk::PresentModeKHR::eFifo) // todo: implement other present modes
      .setPreTransform(vk::SurfaceTransformFlagBitsKHR::eIdentity)
      .setQueueFamilyIndexCount(0)
      .setSurface(surface->GetVkSurface());

  auto result = context->GetVkDevice().createSwapchainKHR(info, adaptor->GetVkAllocator());
  if(result.result != vk::Result::eSuccess)
  {
    VkLog("[VULKAN] Swapchain object creation FAILED!");
    return false;
  }

  mSwapchain = result.value;
  VkLog("[VULKAN] Swapchain object created successfuly!");

  mSwapchain = result.value;

  auto imagesResult = vkDevice.getSwapchainImagesKHR( mSwapchain );
  VkAssert( imagesResult, "Swapchain didn't return images!");
  auto images = imagesResult.value;

  // for each image create view and set layout transition
  for( auto& image : images )
  {

  }


  return true;
}

vk::SwapchainKHR VulkanSwapchain::GetVkSwapchain() const
{
  return mSwapchain;
}
}
}
}
}
