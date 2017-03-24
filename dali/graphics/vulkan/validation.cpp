//
// Created by adam.b on 23/03/17.
//

#include "instance.h"
#include "validation.h"

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

namespace
{

const char *VALIDATION_LAYERS[] = {
"VK_LAYER_LUNARG_screenshot", // screenshot
"VK_LAYER_LUNARG_parameter_validation", // parameter
"VK_LAYER_LUNARG_vktrace", // vktrace ( requires vktrace connection )
"VK_LAYER_LUNARG_monitor", // monitor
"VK_LAYER_LUNARG_swapchain", // swapchain
"VK_LAYER_GOOGLE_threading", // threading
"VK_LAYER_LUNARG_api_dump", // api
"VK_LAYER_LUNARG_object_tracker", // objects
"VK_LAYER_LUNARG_core_validation", // core
"VK_LAYER_GOOGLE_unique_objects", // unique objects
"VK_LAYER_LUNARG_standard_validation", // standard
};
}

class ValidationImpl
{

};

Validation::Validation( Instance& instance )
: mImpl{ new ValidationImpl{} }
{

}



}
}
}

