//
// Created by adam.b on 06/04/17.
//

#include <dali/graphics/vulkan/common.h>
#include <dali/graphics/vulkan/internal/vulkan-command-buffer.h>
#include <dali/graphics/vulkan/internal/vulkan-context.h>

#include "vulkan-command-pool.h"

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{

struct VulkanCommandPoolImpl
{
  VulkanCommandPoolImpl(VulkanContext& context) : mVulkanContext(context)
  {
  }
  ~VulkanCommandPoolImpl() = default;
  bool mExclusive{false};
  bool mResetFlag{false};
  bool mTransientFlag{false};

  VulkanContext& mVulkanContext;

  // vulkan specific
  vk::CommandPool           mCommandPool{nullptr};
  vk::CommandPoolCreateInfo mCreateInfo{};
  std::thread::id           mThreadId; // used if exclusive
};

VulkanCommandPool VulkanCommandPool::New(VulkanContext& context, QueueType type, bool isExclusive,
                                         bool createTransient, bool createResetCommandBuffer)
{
  return VulkanCommandPool(context, type, isExclusive, createTransient, createResetCommandBuffer);
}

VulkanCommandPool::VulkanCommandPool(VulkanContext& context, QueueType queueType, bool isExclusive,
                                     bool createTransient, bool createResetCommandBuffer)
{
  mImpl                 = new VulkanCommandPoolImpl( context );
  mImpl->mExclusive     = isExclusive;
  mImpl->mResetFlag     = createResetCommandBuffer;
  mImpl->mTransientFlag = createTransient;

  vk::CommandPoolCreateFlags createFlags{};
  if(createResetCommandBuffer)
  {
    createFlags |= vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
  }
  if(createTransient)
  {
    createFlags |= vk::CommandPoolCreateFlagBits::eTransient;
  }

  auto adaptor = context.GetGraphicsAdaptor().GetObjectAs< VulkanAdaptor >();

  mImpl->mCreateInfo.setFlags(createFlags).setQueueFamilyIndex(adaptor->GetQueueFamilyIndex(queueType));

  VkAssertCall(context.GetVkDevice().createCommandPool(&mImpl->mCreateInfo, adaptor->GetVkAllocator(),
                                                       &mImpl->mCommandPool));
}

VulkanCommandPool::VulkanCommandPool() = default;

VulkanCommandPool::~VulkanCommandPool()
{
  /*
  if(mImpl->mCommandPool)
  {
    mImpl->mVulkanContext.GetVkDevice().destroyCommandPool(mImpl->mCommandPool);
  }
   */
}

vk::CommandPool VulkanCommandPool::GetVkCommandPool() const
{
  return mImpl->mCommandPool;
}

const vk::CommandPoolCreateInfo& VulkanCommandPool::GetVkCommandPoolCreateInfo() const
{
  return mImpl->mCreateInfo;
}

VulkanContext& VulkanCommandPool::GetVulkanContext() const
{
  return mImpl->mVulkanContext;
}

std::thread::id VulkanCommandPool::GetThreadId() const
{
  return mImpl->mThreadId;
}

void VulkanCommandPool::ThreadAttach()
{
  if(mImpl->mExclusive)
  {
    mImpl->mThreadId = std::this_thread::get_id();
  }
}

void VulkanCommandPool::ThreadDetach()
{
  //mThreadId = nullptr;
}

VulkanCommandBufferArray VulkanCommandPool::AllocateCommandBuffers(int count, bool primary)
{
  if(mImpl->mExclusive)
  {
    // todo check the thread availability
  }
  return std::move(VulkanCommandBuffer::New(*this, primary, count));
}

VulkanCommandBuffer VulkanCommandPool::AllocateCommandBuffer(bool primary)
{
  if(mImpl->mExclusive)
  {
    // todo check the thread availability
  }
  return std::move(VulkanCommandBuffer::New(*this, primary));
}


VulkanCommandPool::operator bool()
{
  return mImpl != nullptr;
}
}
}
}
}
