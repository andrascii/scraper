#pragma once

#include "logger.h"

namespace common {

template <typename Expected, typename Error>
using Expected = tl::expected<Expected, Error>;

template <typename Error>
using Unexpected = tl::unexpected<Error>;

class Helpers final {
 public:
  static void SetCurrentThreadName(const std::string& name) noexcept;
};

}
