//
// Created by adam.b on 03/04/17.
//

#include "graphics-swapchain-impl-base.h"

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
namespace Internal
{

GraphicsSwapchainImplBase::GraphicsSwapchainImplBase(const GraphicsContext &context, const GraphicsSurface &surface)
: mContext( context ), mSurface( surface )
{
}

}
}
}
}