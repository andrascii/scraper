#pragma once

#include "Logger.h"

namespace common {

template <typename Expected, typename Error>
using Expected = tl::expected<Expected, Error>;

template <typename Error>
using Unexpected = tl::unexpected<Error>;

class Helpers final {
 public:
  static void setCurrentThreadName(const std::string& name) noexcept;
};

}
