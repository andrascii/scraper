#pragma once

#include "IApplication.h"
#include "Helpers.h"
#include "Settings.h"
#include "IMessagePublisher.h"

namespace api {

class DbProxyApplication : public IApplication {
 public:
  DbProxyApplication(SharedSettings settings, std::unique_ptr<IMessagePublisher> publisher);

  std::error_code Start() noexcept;

 private:
  SharedSettings settings_;
  std::unique_ptr<IMessagePublisher> publisher_;
};

}
