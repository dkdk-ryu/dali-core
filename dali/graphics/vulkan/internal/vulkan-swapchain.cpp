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

VulkanSwapchain::VulkanSwapchain(const GraphicsContext& context,
                                 const GraphicsSurface& surface,
                                 uint32_t               bufferCount,
                                 DepthStencil           depthStencil,
                                 bool                   enforceVSync)
: GraphicsSwapchainImplBase(context, surface), mDepthStencilMode{depthStencil}, mEnforceVSync{enforceVSync}, mMinBufferCount{bufferCount}
{
}

/**
 * When one time initialising the swapchain we don't want to over-complicate it. It's not
 * where the performance is needed so we can go "commando" and simply wait device
 * idle until memory barriers execute ( in other case don't do it! )
 *
 */
bool VulkanSwapchain::Initialise()
{
  auto adaptor = mContext.GetGraphicsAdaptor().GetObjectAs< VulkanAdaptor >();
  // fixme: turn device into VulkanHandle for future requirement
  auto context = mContext.GetObjectAs< VulkanContext >();

  auto surface = mSurface.GetObjectAs< VulkanSurface >();

  auto vkInstance  = adaptor->GetVkInstance();
  auto vkDevice    = context->GetVkDevice();
  auto vkAllocator = adaptor->GetVkAllocator();

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
      .setMinImageCount(mMinBufferCount)
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

  auto dsData = CreateDepthStencil();

  // depth/stencil
  mDepthStencil.image     = std::get< 0 >(dsData);
  mDepthStencil.imageView = std::get< 1 >(dsData);
  mDepthStencil.memory    = std::get< 2 >(dsData);

  auto imagesResult = vkDevice.getSwapchainImagesKHR(mSwapchain);
  VkAssert(imagesResult, "Swapchain didn't return images!");
  auto images = imagesResult.value;

  // for each image create view and set layout transition
  vk::FenceCreateInfo     fenceInfo;
  vk::ImageViewCreateInfo ivInfo;
  ivInfo.setFormat(surface->GetVkSurfaceFormat().format);
  ivInfo.setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG,
                                            vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA));
  ivInfo.setSubresourceRange(vk::ImageSubresourceRange()
                                 .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                 .setBaseArrayLayer(0)
                                 .setBaseMipLevel(0)
                                 .setLayerCount(1)
                                 .setLevelCount(1));
  ivInfo.setViewType(vk::ImageViewType::e2D);

  // create layout transition barriers ( layout attachment optimal ) to
  // set images in the initial proper ready to render state
  // make base barrier entry
  vk::ImageMemoryBarrier barrier;
  barrier.setSubresourceRange(vk::ImageSubresourceRange()
                                  .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                  .setBaseArrayLayer(0)
                                  .setBaseMipLevel(0)
                                  .setLayerCount(1)
                                  .setLevelCount(1));

  barrier.setOldLayout(vk::ImageLayout::eUndefined);
  barrier.setNewLayout(vk::ImageLayout::eColorAttachmentOptimal);
  barrier.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

  std::vector< vk::ImageMemoryBarrier > barriers;

  for(auto& image : images)
  {
    SwapchainImage img;
    img.aspectFlags = vk::ImageAspectFlagBits::eColor;
    img.layout      = vk::ImageLayout::eUndefined;
    img.image       = image;

    img.depthStencilSharedMode = true;

    // update barriers
    barrier.setImage(image);
    barriers.push_back(barrier);

    // create image view
    ivInfo.setImage(image);
    VkAssertCall(vkDevice.createImageView(&ivInfo, vkAllocator, &img.imageView));
    VkAssertCall(vkDevice.createFence(&fenceInfo, vkAllocator, &img.fence));

    mImages.push_back(img);
  }

  // if DS add one more barrier
  if(mDepthStencilMode != DepthStencil::NONE)
  {
    barrier.setNewLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
    barrier.setImage(mDepthStencil.image);
    // todo: eDepth|eStencil
    barrier.setSubresourceRange(vk::ImageSubresourceRange()
                                    .setAspectMask(vk::ImageAspectFlagBits::eDepth)
                                    .setBaseArrayLayer(0)
                                    .setBaseMipLevel(0)
                                    .setLayerCount(1)
                                    .setLevelCount(1));
    barrier.setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite |
                             vk::AccessFlagBits::eDepthStencilAttachmentRead |
                             vk::AccessFlagBits::eDepthStencilAttachmentWrite);
    barriers.push_back(barrier);
  }
  // record pipeline barriers
  VulkanCommandPool pool = context->GetMainCommandPool();
  auto buffer = pool.AllocateCommandBuffer( true );

  assert( buffer.Begin( true ) );

  auto vkBuffer = buffer.GetVkBuffer();

  vkBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTopOfPipe,
                         vk::DependencyFlags{}, 0, nullptr, 0, nullptr, barriers.size(), barriers.data());

  buffer.End();

  // submit to queue ( no need to fence here, app is just starting, it
  // may be safer to do the same when replacing swapchain )
  auto           queue = context->GetVkQueue(0, QueueType::GRAPHICS);
  vk::SubmitInfo submitInfo{};
  submitInfo.setCommandBufferCount(1);
  submitInfo.setPCommandBuffers(&vkBuffer);
  VkAssertCall(queue.submit(submitInfo, nullptr));

  // wait idle
  queue.waitIdle();

  // fixme: free command buffer

  // prepare first framebuffer
  CreateFramebuffers();

  // acquire image from swapchain
  vk::Fence fence;
  VkAssertCall(vkDevice.createFence(&fenceInfo, vkAllocator, &fence));
  {
    auto result = VkTestCall(vkDevice.acquireNextImageKHR(mSwapchain, 1000, nullptr, fence, &mCurrentImage));
    // success states
    if( result == vk::Result::eTimeout )
    {
    // todo
    }
    else if (result == vk::Result::eSuboptimalKHR)
    {
    // todo
    } // not ready

    //wait for fence
    while((VkTestCall(vkDevice.waitForFences(1, &fence, true, 16))) == vk::Result::eTimeout)
      ;
    vkDevice.destroyFence( fence, vkAllocator );
  }
  if(mCurrentImage == -1u)
  {
    // todo: cleanup
    return false;
  }

  return true;
}

vk::SwapchainKHR VulkanSwapchain::GetVkSwapchain() const
{
  return mSwapchain;
}

bool VulkanSwapchain::CreateFramebuffers()
{
  auto adaptor = mContext.GetGraphicsAdaptor().GetObjectAs< VulkanAdaptor >();
  auto context = mContext.GetObjectAs< VulkanContext >();

  auto surface = mSurface.GetObjectAs< VulkanSurface >();

  auto vkInstance  = adaptor->GetVkInstance();
  auto vkDevice    = context->GetVkDevice();
  auto vkAllocator = adaptor->GetVkAllocator();

  int attachmentCount = mDepthStencilMode != DepthStencil::NONE ? 2 : 1;

  auto depthImageView = mDepthStencil.imageView;

  vk::AttachmentDescription attDesc[2];

  // color attachment
  attDesc[0]
      .setFormat(mSurface.GetObjectAs< VulkanSurface >()->GetVkSurfaceFormat().format)
      .setFinalLayout(vk::ImageLayout::ePresentSrcKHR)
      .setInitialLayout(vk::ImageLayout::eColorAttachmentOptimal)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setSamples(vk::SampleCountFlagBits::e1)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setStoreOp(vk::AttachmentStoreOp::eDontCare);

  // depth stencil attachment ( if enabled )
  attDesc[1]
      .setFormat(vk::Format::eD16Unorm)
      .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
      .setInitialLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
      .setLoadOp(vk::AttachmentLoadOp::eClear)
      .setSamples(vk::SampleCountFlagBits::e1)
      .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
      .setStoreOp(vk::AttachmentStoreOp::eDontCare);

  vk::AttachmentReference attRef[2];
  attRef[0].setAttachment(0).setLayout(vk::ImageLayout::eColorAttachmentOptimal);
  attRef[1].setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

  vk::SubpassDescription subpass;
  subpass.setColorAttachmentCount(1)
      .setInputAttachmentCount(0)
      .setPColorAttachments(attRef)
      .setPDepthStencilAttachment(mDepthStencilMode != DepthStencil::NONE ? &attRef[1] : nullptr)
      .setPInputAttachments(nullptr)
      .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
      .setPPreserveAttachments(nullptr)
      .setPreserveAttachmentCount(0)
      .setPResolveAttachments(nullptr);

  // renderpass info
  vk::RenderPassCreateInfo rpInfo;
  rpInfo.setPAttachments(attDesc);
  rpInfo.setAttachmentCount(attachmentCount);
  rpInfo.setDependencyCount(0);
  rpInfo.setPDependencies(nullptr);
  rpInfo.setPSubpasses(&subpass);
  rpInfo.setSubpassCount(1);

  vk::RenderPass mainRenderPass;
  VkAssertCall(vkDevice.createRenderPass(&rpInfo, vkAllocator, &mainRenderPass));

  vk::ImageView attachments[2] = {nullptr, depthImageView};

  vk::FramebufferCreateInfo info;
  info.setAttachmentCount(attachmentCount);
  info.setWidth(mSurface.GetWidth());
  info.setHeight(mSurface.GetHeight());
  info.setLayers(1);
  info.setPAttachments(attachments);
  info.setRenderPass(mainRenderPass);

  for(auto& buf : mImages)
  {
    // renderpass
    buf.renderpass = mainRenderPass;

    // framebuffer
    attachments[0] = buf.imageView;
    VkAssertCall(vkDevice.createFramebuffer(&info, vkAllocator, &buf.framebuffer));
  }

  return true;
}

ImageData VulkanSwapchain::CreateDepthStencil()
{
  auto adaptor     = mContext.GetGraphicsAdaptor().GetObjectAs< VulkanAdaptor >();
  auto context     = mContext.GetObjectAs< VulkanContext >();
  auto vkDevice    = context->GetVkDevice();
  auto vkAllocator = adaptor->GetVkAllocator();

  if(mDepthStencilMode == DepthStencil::NONE)
  {
    return ImageData{nullptr, nullptr, nullptr}; // true, no need to create anything
  }

  vk::Image        dsImage;
  vk::ImageView    dsImageView;
  vk::DeviceMemory dsMemory;

  vk::ImageCreateInfo info;
  info.setSamples(vk::SampleCountFlagBits::e1)
      .setInitialLayout(vk::ImageLayout::eUndefined)
      .setFormat(vk::Format::eD16Unorm)
      .setQueueFamilyIndexCount(0)
      .setPQueueFamilyIndices(nullptr)
      .setArrayLayers(1)
      .setExtent({( uint32_t )mSurface.GetWidth(), ( uint32_t )mSurface.GetHeight(), 1})
      .setImageType(vk::ImageType::e2D)
      .setMipLevels(1)
      .setSharingMode(vk::SharingMode::eExclusive)
      .setTiling(vk::ImageTiling::eOptimal)
      .setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment);

  VkAssertCall(vkDevice.createImage(&info, vkAllocator, &dsImage));

  dsMemory = context->AllocateMemory(dsImage, vk::MemoryPropertyFlagBits::eDeviceLocal, true);

  vk::ImageViewCreateInfo ivInfo;
  ivInfo.setFormat(vk::Format::eD16Unorm)
      .setImage(dsImage)
      .setSubresourceRange(vk::ImageSubresourceRange()
                               .setLevelCount(1)
                               .setLayerCount(1)
                               .setBaseMipLevel(0)
                               .setBaseArrayLayer(0)
                               .setAspectMask(vk::ImageAspectFlagBits::eDepth)); //@todo: support stencil!

  VkAssertCall(vkDevice.createImageView(&ivInfo, vkAllocator, &dsImageView));

  return std::make_tuple(dsImage, dsImageView, dsMemory);
}

void VulkanSwapchain::SwapBuffers(bool vsync)
{
  auto adaptor = mContext.GetGraphicsAdaptor().GetObjectAs< VulkanAdaptor >();
  auto context = mContext.GetObjectAs< VulkanContext >();

  auto surface = mSurface.GetObjectAs< VulkanSurface >();

  auto vkInstance  = adaptor->GetVkInstance();
  auto vkDevice    = context->GetVkDevice();
  auto vkAllocator = adaptor->GetVkAllocator();

  // PresentINfo

  vk::Queue queue = context->GetVkQueue( 0, QueueType::PRESENT );
  vk::PresentInfoKHR info;

  info
    .setPImageIndices( &mCurrentImage )
    .setPResults( nullptr )
    .setPSwapchains( &mSwapchain )
    .setPWaitSemaphores( nullptr )
    .setSwapchainCount( 1 )
    .setWaitSemaphoreCount( 0 );

  queue.presentKHR( &info );
  // acquire next image from swapchain
  vk::FenceCreateInfo fenceInfo{};
  vk::Fence fence;
  VkAssertCall(vkDevice.createFence(&fenceInfo, vkAllocator, &fence));
  {
    auto result = VkTestCall(vkDevice.acquireNextImageKHR(mSwapchain, 1000, nullptr, fence, &mCurrentImage));
    // success states
    if( result == vk::Result::eTimeout )
    {
      // todo
    }
    else if (result == vk::Result::eSuboptimalKHR)
    {
      // todo
    } // not ready

    //wait for fence
    while((VkTestCall(vkDevice.waitForFences(1, &fence, true, 1000000))) == vk::Result::eTimeout)
      ;
    VkLog("[SWAPCHAIN] Fence destroyed!");
    vkDevice.destroyFence( fence, vkAllocator );
  }

  VkLog("[SWAPCHAIN] Current buffer: %u", mCurrentImage);

}
}
}
}
}
