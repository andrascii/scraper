#pragma once

#include "IApplication.h"
#include "Helpers.h"
#include "Settings.h"
#include "IMessagePublisher.h"
#include "IMigrator.h"

namespace api {

class DbProxyApplication : public IApplication {
 public:
  DbProxyApplication(
    SharedSettings settings,
    //std::unique_ptr<IMessagePublisher> publisher,
    std::vector<std::unique_ptr<IMigrator>> migrators);

  std::error_code Start();

 private:
  void StartHttpServer() const;

 private:
  SharedSettings settings_;
  std::unique_ptr<IMessagePublisher> publisher_;
  std::vector<std::unique_ptr<IMigrator>> migrators_;
};

}
