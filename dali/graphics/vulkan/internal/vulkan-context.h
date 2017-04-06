//
// Created by adam.b on 31/03/17.
//

#ifndef DALI_CORE_VULKAN_CONTEXT_H
#define DALI_CORE_VULKAN_CONTEXT_H

#include <dali/graphics/vulkan/common.h>
#include <dali/graphics/vulkan/internal/graphics-context-impl-base.h>
#include <dali/graphics/vulkan/internal/vulkan-adaptor.h>
#include <dali/graphics/vulkan/graphics-swapchain.h>
#include <vulkan/vulkan.hpp>
#include <dali/graphics/vulkan/internal/vulkan-command-pool.h>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
class GraphicsAdaptor;
class GraphicsSurface;

namespace Internal
{

class VulkanCommandBuffer;

using QueueArray = std::vector<vk::Queue>;
using FamilyQueueArray = std::vector<QueueArray>;

class VulkanContext : public Internal::GraphicsContextImplBase
{
public:
  VulkanContext(const GraphicsAdaptor& adaptor, const GraphicsSurface& surface, const ExtensionNameList& extensions);
  virtual ~VulkanContext() = default;

  virtual GraphicsSwapchain CreateSwapchain(const GraphicsSurface& surface,
                                            uint32_t               bufferCount,
                                            DepthStencil           depthStencil,
                                            bool                   enforceVSync);

  virtual bool Initialise();

  vk::Device GetVkDevice() const;

  vk::Queue GetVkQueue( int index, QueueType type );

  int32_t GetMemoryIndex(uint32_t memoryTypeBits, vk::MemoryPropertyFlags properties);

  vk::DeviceMemory AllocateMemory( vk::Image image, vk::MemoryPropertyFlags flags, bool doBind );
  vk::DeviceMemory AllocateMemory( vk::Buffer buffer, vk::MemoryPropertyFlags flags, bool doBind );

  vk::ImageMemoryBarrier ChangeImageLayout( vk::Image image,
                                vk::PipelineStageFlags stageSrc,
                                vk::ImageLayout layoutSrc,
                                vk::PipelineStageFlags stageDst,
                                vk::ImageLayout layoutDst );

  // todo: there may be not such thing like "main command pool"
  VulkanCommandPool GetMainCommandPool();


private:

  vk::Device mVkDevice { nullptr };

  FamilyQueueArray mVkQueueArray; // queues allocated per family

  ExtensionNameList          mEnabledExtensionCStr;
  std::vector< std::string > mEnabledExtensionStr;

  // transient pool is used for quick commands allocation and deallocation
  // like using with barriers etc.
  //vk::CommandPool mTransientPool { nullptr };
  VulkanCommandPool mTransientPool;
};

} // Internal
} // Vulkan
} // Graphics
} // Dali

#endif //DALI_CORE_GRAPHICSCONTEXTIMPL_H
