#pragma once

#include "DbProxySettings.h"

namespace api {

class PgConnection {
public:
  PgConnection(SharedDbProxySettings settings);

private:
  std::shared_ptr<PGconn> connection_;
};

}