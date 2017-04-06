//
// Created by adam.b on 06/04/17.
//

#ifndef DALI_CORE_VULKAN_COMMAND_POOL_H
#define DALI_CORE_VULKAN_COMMAND_POOL_H

#include <dali/graphics/vulkan/common.h>
#include <dali/graphics/vulkan/internal/vulkan-adaptor.h>
#include <dali/graphics/vulkan/internal/vulkan-command-buffer.h>
#include <dali/graphics/vulkan/internal/vulkan-command-pool.h>
#include <thread>
namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{

using VulkanCommandBufferArray = std::vector< VulkanCommandBuffer >;

class VulkanContext;
class VulkanAdaptor;
class VulkanCommandPool
{
private:
  VulkanCommandPool(VulkanContext& context, QueueType type, bool isExclusive, bool createTransient,
                    bool createResetCommandBuffer);

public:
  VulkanCommandPool();
  ~VulkanCommandPool();

  static VulkanCommandPool New(VulkanContext& context, QueueType type, bool isExclusive,
                               bool createTransient, bool createResetCommandBuffer);

  vk::CommandPool                  GetVkCommandPool() const;
  VulkanContext&                   GetVulkanContext() const;
  const vk::CommandPoolCreateInfo& GetVkCommandPoolCreateInfo() const;
  std::thread::id                  GetThreadId() const;

  // attaching to thread will make sure that any attempt of using pool on the
  // wrong thread will assert
  void ThreadAttach();
  void ThreadDetach();

  VulkanCommandBufferArray AllocateCommandBuffers(int count, bool primary);
  VulkanCommandBuffer AllocateCommandBuffer(bool primary);

  operator bool();
  bool operator!() const
  {
    return mImpl == nullptr;
  }

private:
  struct VulkanCommandPoolImpl* mImpl{nullptr};
};
}
}
}
}

#endif //DALI_CORE_VULKANCOMMANDPOOL_H
