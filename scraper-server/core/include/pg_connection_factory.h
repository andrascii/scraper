#pragma once

#include "settings.h"

namespace core {

class PgConnectionFactory final {
 public:

  static std::shared_ptr<pqxx::connection> Create(const std::shared_ptr<Settings>& settings);
};

}// namespace core