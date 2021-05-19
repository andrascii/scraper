#pragma once

#include "IApplication.h"
#include "Helpers.h"
#include "DbProxySettings.h"

namespace api {

class DbProxyApplication : public IApplication {
public:
  DbProxyApplication(SharedDbProxySettings settings);

  std::error_code Start() noexcept;

private:
  SharedDbProxySettings settings_;
};

}
