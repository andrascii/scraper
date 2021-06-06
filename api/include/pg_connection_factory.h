#pragma once

#include "settings.h"

namespace api {

class PgConnectionFactory final {
 public:
  static std::shared_ptr<pqxx::connection> Create(const std::shared_ptr<Settings>& settings);
};

}