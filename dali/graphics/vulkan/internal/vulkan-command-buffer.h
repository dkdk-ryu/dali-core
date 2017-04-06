//
// Created by adam.b on 06/04/17.
//

#ifndef DALI_CORE_VULKANCOMMANDBUFFER_H
#define DALI_CORE_VULKANCOMMANDBUFFER_H

#include <thread>
#include <vulkan/vulkan.hpp>
namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{
class VulkanCommandPool;
class VulkanCommandBuffer
{
private:
  VulkanCommandBuffer(const VulkanCommandPool& pool, bool isPrimary, vk::CommandBuffer buffer = nullptr);

  void SetVkCommandBuffer(vk::CommandBuffer cmdBuf);

public:

  enum class State
  {
    UNDEFINED,
    CREATED,
    RESET,
    RECORDING,
    RECORDED,
    SUBMITTED
  };

  static std::vector< VulkanCommandBuffer > New(const VulkanCommandPool& pool, bool isPrimary, int count);
  static VulkanCommandBuffer New(const VulkanCommandPool& pool, bool isPrimary);

  ~VulkanCommandBuffer() = default;

  vk::CommandBuffer GetVkBuffer() const;
  VulkanCommandPool&   GetVkCommandPool() const;
  std::thread::id   GetThreadId() const;

  State GetState() const;

  bool Begin( bool oneTimeSubmit, bool renderPassContinue = false, bool simultaneousUse = false);
  bool End();
  bool Free();
  bool Reset();

private:
  struct VulkanCommandBufferImpl* mImpl{nullptr};
};
}
}
}
}

#endif //DALI_CORE_VULKANCOMMANDBUFFER_H
