#include "surface.h"

#include <vulkan/vulkan.hpp>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

struct SurfaceImpl
{
  vk::SurfaceKHR              surface;
  vk::SurfaceFormatKHR        format;
  vk::SurfaceCapabilitiesKHR  caps;
};

}
}
}
