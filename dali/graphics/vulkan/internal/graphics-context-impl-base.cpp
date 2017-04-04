//
// Created by adam.b on 03/04/17.
//

#include "graphics-context-impl-base.h"

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
class GraphicsSurface;
namespace Internal
{
GraphicsContextImplBase::GraphicsContextImplBase( const GraphicsAdaptor& adaptor, const GraphicsSurface& surface, const ExtensionNameList& extensions)
: mGraphicsAdaptor( adaptor ), mSurface( surface )
{

}

GraphicsAdaptor& GraphicsContextImplBase::GetGraphicsAdaptor()
{
  return mGraphicsAdaptor;
}

GraphicsSurface& GraphicsContextImplBase::GetSurface()
{
  return mSurface;
}


}
}
}
}
