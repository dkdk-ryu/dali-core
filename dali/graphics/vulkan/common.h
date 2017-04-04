//
// Created by adam.b on 27/03/17.
//

#ifndef DALI_CORE_COMMON_H
#define DALI_CORE_COMMON_H

#include <vulkan/vulkan.hpp>

#include <cinttypes>
#include <memory>
#include <cassert>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{

#define VkAssert( var, message ) \
{\
if( var.result != vk::Result::eSuccess )\
{\
VkLog("[Assert] %s", message );\
assert( var.result == vk::Result::eSuccess );\
}\
}

void VkLog(const char *format, ...);

// helper template to create relation between flags and bits
template< typename BitType, typename FlagType = uint32_t >
struct BitFlags
{
  BitFlags() = default;

  BitFlags(FlagType f) : flags(f)
  {
  }

  BitFlags(const BitType &bit)
  {
    flags = static_cast< FlagType >(bit);
  }

  operator FlagType()
  {
    return flags;
  }

  bool operator==(FlagType srcFlags) const
  {
    return srcFlags == flags;
  }

  bool operator!=(const FlagType &srcFlags) const
  {
    return srcFlags != flags;
  }

  BitFlags operator|(const BitType &bit)
  {
    return BitFlags(flags | static_cast< FlagType >(bit));
  }

  BitFlags &operator|=(const BitType &bit)
  {
    flags |= static_cast< FlagType >(bit);
    return *this;
  }

  BitFlags operator&(const BitFlags &rhs) const
  {
    return FlagType{(rhs.flags & flags)};
  }

  BitFlags operator&(unsigned int rhs) const
  {
    return BitFlags{(rhs & flags)};
  }

  FlagType flags{0};
};

template< typename T >
using UPtr = std::unique_ptr< T >;

using ExtensionNameList = std::vector< const char * >;
using LayerNameList     = std::vector< const char * >;

// fixme temporary workaround to stop passing references around
template< typename T >
class Handle
{
public:
  Handle() = default;
  Handle(T *ptr) : mObject(ptr)
  {
  }
  Handle(const Handle &) = default;
  Handle(Handle &)       = default;
  Handle(Handle &&)      = default;
  Handle operator=(Handle &&) = delete;
  ~Handle()                   = default;

  T *operator->()
  {
    return mObject;
  }

  operator bool()
  {
    return (mObject != nullptr);
  }

  template< typename P >
  P *StaticCast()
  {
    return static_cast< P * >(mObject);
  }

  template< typename P >
  P *DynamicCast()
  {
    return dynamic_cast< P * >(mObject);
  }

  T* GetObject() const
  {
    return mObject;
  }

  template <typename K>
  K* GetObjectAs() const
  {
    return static_cast<K*>(mObject);
  }

protected:

  void SetObject( T* object )
  {
    mObject = object;
  }

private:
  T *mObject { nullptr };
};

// common for adaptor
enum class ValidationLayerBit2
{
  NONE                 = 0,
  SCREENSHOT           = (1 << 0),
  PARAMETER_VALIDATION = (1 << 1),
  VKTRACE              = (1 << 2),
  MONITOR              = (1 << 3),
  SWAPCHAIN            = (1 << 4),
  THREADING            = (1 << 5),
  API_DUMP             = (1 << 6),
  OBJECT_TRACKER       = (1 << 7),
  CORE_VALIDATION      = (1 << 8),
  UNIQUE_OBJECTS       = (1 << 9),
  STANDARD_VALIDATION  = (1 << 10),
  ALL                  = 0xFFFF
};

enum class ValidationChannelBit
{
  NONE                = 0,
  INFORMATION         = (1 << 0),
  WARNING             = (1 << 1),
  PERFORMANCE_WARNING = (1 << 2),
  ERROR               = (1 << 3),
  DEBUG               = (1 << 4),
  ALL                 = 0xFF
};

using ValidationLayerFlags2     = BitFlags< ValidationLayerBit2 >;
using ValidationChannelFlags    = BitFlags< ValidationChannelBit >;

enum class PhysicalDeviceBit
{
  // physical device type
  ANY = 0,
  DISCRETE = (1 << 0),
  INTEGRATED = (1 << 1)
};

using PhysicalDeviceFlags = BitFlags< PhysicalDeviceBit >;

}
}
}

#endif //DALI_CORE_COMMON_H
