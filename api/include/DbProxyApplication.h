#pragma once

#include "IApplication.h"
#include "Helpers.h"
#include "DbProxySettings.h"

namespace api {

class DbProxyApplication : public IApplication {
public:
  DbProxyApplication(DbProxySettings  settings);

  std::error_code Start() noexcept;

private:
  DbProxySettings settings_;
};

}
