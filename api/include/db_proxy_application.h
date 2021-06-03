#pragma once

#include "iapplication.h"
#include "helpers.h"
#include "settings.h"
#include "imessage_publisher.h"
#include "imigrator.h"

namespace api {

class DbProxyApplication : public IApplication {
 public:
  DbProxyApplication(
    SharedSettings settings,
    //std::unique_ptr<IMessagePublisher> publisher,
    std::vector<std::unique_ptr<IMigrator>> migrators);

  std::error_code Start();
  void Stop() noexcept;

 private:
  std::error_code StartHttpServer();

 private:
  SharedSettings settings_;
  std::unique_ptr<IMessagePublisher> publisher_;
  std::vector<std::unique_ptr<IMigrator>> migrators_;
  boost::asio::io_context ctx_;
};

}
