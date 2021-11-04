#pragma once

#include "logger.h"
#include "types.h"

namespace common {

class Helpers final {
 public:

  static void SetCurrentThreadName(const std::string& name) noexcept;
};

}// namespace common
