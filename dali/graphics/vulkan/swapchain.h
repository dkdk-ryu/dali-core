#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

class Swapchain
{
public:

  Swapchain();
  ~Swapchain() = default;
  Swapchain( const Swapchain&& ) = delete;
  Swapchain( const Swapchain& ) = delete;


};

}
}
}

#endif // SWAPCHAIN_H
