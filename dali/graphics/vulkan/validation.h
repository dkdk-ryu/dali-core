//
// Created by adam.b on 23/03/17.
//

#ifndef DALI_CORE_VALIDATION_H
#define DALI_CORE_VALIDATION_H

#include <cinttypes>

namespace Dali
{
namespace Graphics
{
namespace Vulkan
{
enum class ValidationBit
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

  BitFlags operator|(const BitType &bit)
  {
    return BitFlags(flags | static_cast< FlagType >(bit));
  }

  BitFlags &operator|=(const BitType &bit)
  {
    flags |= static_cast< FlagType >(bit);
    return *this;
  }

  FlagType flags{0};
};

using ValidationFlags          = BitFlags< ValidationBit >;
using ValidationChannelFlags   = BitFlags< ValidationChannelBit >;
using ValidationLayerNameTable = std::vector< const char * >;

class ValidationImpl;
class Instance;

class Validation
{
public:
  Validation() = delete;

  Validation(Instance &instance);

  ~Validation() = default;

public:
  void Enable();

  void Disable();

  void SetChannels(const ValidationChannelFlags &flags);

  ValidationLayerNameTable GetNameTable(const ValidationFlags &flags);

private:
  std::unique_ptr< ValidationImpl > mImpl;
};
}
}
}

#endif //DALI_CORE_VALIDATION_H
