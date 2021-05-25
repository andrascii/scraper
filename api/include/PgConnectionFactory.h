#pragma once

#include "Settings.h"

namespace api {

using SharedPgConnection = std::shared_ptr<pqxx::connection>;

class PgConnectionFactory final {
 public:
  static SharedPgConnection Create(const SharedSettings& settings);
};

}