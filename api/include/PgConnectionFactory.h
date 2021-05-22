#pragma once

#include "Settings.h"

namespace api {

using SharedPgConnection = std::shared_ptr<PGconn>;

class PgConnectionFactory final {
public:
  static SharedPgConnection Create(const SharedSettings& settings);
};

}