//
// Created by adam.b on 06/04/17.
//

#include "vulkan-command-buffer.h"
#include <dali/graphics/vulkan/internal/vulkan-command-pool.h>
#include <dali/graphics/vulkan/internal/vulkan-context.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
using State = VulkanCommandBuffer::State;
struct VulkanCommandBufferImpl
{
  VulkanCommandBufferImpl(VulkanContext& context, const VulkanCommandPool& pool)
  : mPool(pool), mContext(context), mBuffer(nullptr), mLevel(vk::CommandBufferLevel::ePrimary)
  {
  }

  VulkanCommandPool      mPool;
  VulkanContext&         mContext;
  vk::CommandBuffer      mBuffer;
  vk::CommandBufferLevel mLevel;

  // state of command buffer
  State mState{State::UNDEFINED};
};

VulkanCommandBufferArray VulkanCommandBuffer::New(const VulkanCommandPool& pool, bool isPrimary, int count)
{
  auto vkDevice = pool.GetVulkanContext().GetVkDevice();

  std::vector< vk::CommandBuffer > buffers;
  buffers.resize(count);

  vk::CommandBufferAllocateInfo info;
  info.setCommandBufferCount(count)
      .setCommandPool(pool.GetVkCommandPool())
      .setLevel(isPrimary ? vk::CommandBufferLevel::ePrimary : vk::CommandBufferLevel::eSecondary);

  VkAssertCall(vkDevice.allocateCommandBuffers(&info, buffers.data()));

  // move buffers data
  VulkanCommandBufferArray output;
  for(auto& cmdbuf : buffers)
  {
    VulkanCommandBuffer tmp(pool, isPrimary, cmdbuf);
    output.emplace_back(tmp);
  }
  return std::move(output);
}

VulkanCommandBuffer VulkanCommandBuffer::New(const VulkanCommandPool& pool, bool isPrimary)
{
  // todo make more efficient implementation
  return VulkanCommandBuffer::New(pool, isPrimary, 1)[0];
}

VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandPool& pool, bool isPrimary, vk::CommandBuffer buffer)
{
  mImpl         = new VulkanCommandBufferImpl(pool.GetVulkanContext(), pool);
  mImpl->mLevel = isPrimary ? vk::CommandBufferLevel::ePrimary : vk::CommandBufferLevel::eSecondary;
  mImpl->mBuffer = buffer;
}

void VulkanCommandBuffer::SetVkCommandBuffer(vk::CommandBuffer cmdBuf)
{
  mImpl->mBuffer = cmdBuf;
}

bool VulkanCommandBuffer::Begin(bool oneTimeSubmit, bool renderPassContinue, bool simultaneousUse)
{
  // check state
  if(mImpl->mState != State::CREATED && mImpl->mState != State::UNDEFINED && mImpl->mState != State::RESET)
  {
    VkLog("[VKCMDBUF] Invalid buffer state: %d", static_cast< int >(mImpl->mState));
    return false;
  }
  auto vkDevice = mImpl->mContext.GetVkDevice();

  vk::CommandBufferUsageFlags flags{};
  if(oneTimeSubmit)
    flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
  if(renderPassContinue)
    flags |= vk::CommandBufferUsageFlagBits::eRenderPassContinue;
  if(simultaneousUse)
    flags |= vk::CommandBufferUsageFlagBits::eSimultaneousUse;

  // todo implement inheritance
  vk::CommandBufferBeginInfo info;
  info.setFlags(flags).setPInheritanceInfo(nullptr);

  if(VkTestCall(mImpl->mBuffer.begin(&info)) == vk::Result::eSuccess)
  {
    mImpl->mState = State::RECORDING;
    return true;
  }

  return false;
}

bool VulkanCommandBuffer::End()
{
  // check state
  if(mImpl->mState != State::RECORDING)
  {
    VkLog("[VKCMDBUF] Invalid buffer state: %d, it must be RECORDING", static_cast< int >(mImpl->mState));
    return false;
  }
  auto vkDevice = mImpl->mContext.GetVkDevice();

  if(VkTestCall(mImpl->mBuffer.end()) == vk::Result::eSuccess)
  {
    mImpl->mState = State::RECORDED;
    return true;
  }
  return false;
}

bool VulkanCommandBuffer::Free()
{
}

bool VulkanCommandBuffer::Reset()
{
}

State VulkanCommandBuffer::GetState() const
{
  return mImpl->mState;
}

vk::CommandBuffer VulkanCommandBuffer::GetVkBuffer() const
{
  return mImpl->mBuffer;
}

VulkanCommandPool& VulkanCommandBuffer::GetVkCommandPool() const
{
  return mImpl->mPool;
}

std::thread::id VulkanCommandBuffer::GetThreadId() const
{

}
}
}
}
}
