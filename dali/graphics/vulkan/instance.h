//
// Created by adam.b on 23/03/17.
//

#ifndef DALI_CORE_INSTANCE_H
#define DALI_CORE_INSTANCE_H

#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>
#include "validation.h"
#include "surface.h"
namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

using ExtensionNameList = std::vector< const char* >;
struct InstanceImpl;
class SurfaceInfo;
class Instance;
using InstancePtr = std::unique_ptr<Instance>;
class Instance
{
protected:
  Instance();

public:
  ~Instance();

  static InstancePtr New( const ExtensionNameList& extensions, ValidationFlags validationFlags = ValidationBit::NONE );

  static bool IsExtensionAvailable(const std::string& name);
  static bool IsLayerAvailable(const std::string& name);

  bool CreateSurface( const SurfaceInfo& info );

private:

  bool CreateInstance( const ExtensionNameList& extensions, ValidationFlags validationFlags );
  bool CreateXlibSurface( const SurfaceXlibInfo& info );
  bool CreateXcbSurface( const SurfaceXcbInfo& info );

public:
  vk::Result InitialisePhysicalGPU(bool preferDescreeteNotIntegrated = true);

private:
  std::unique_ptr< InstanceImpl > mImpl;
};
}
}
}

#endif //DALI_CORE_INSTANCE_H
