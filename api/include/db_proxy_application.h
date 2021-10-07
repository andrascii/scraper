#pragma once

#include "iapplication.h"
#include "helpers.h"
#include "settings.h"
#include "imessage_publisher.h"
#include "imigration_factory.h"
#include "pg_connection_pool.h"

namespace api {

class DbProxyApplication : public IApplication {
 public:
  DbProxyApplication(
    std::shared_ptr<Settings> settings,
    //std::unique_ptr<IMessagePublisher> publisher,
    std::shared_ptr<IMigrationFactory> factory
  );

  std::error_code Start();
  void Stop() noexcept;

 private:
  std::error_code StartHttpServer();

 private:
  std::shared_ptr<Settings> settings_;
  std::unique_ptr<IMessagePublisher> publisher_;
  std::shared_ptr<IMigrationFactory> migration_factory_;
  std::shared_ptr<PgConnectionPool> pg_pool_;
  boost::asio::io_context ctx_;
};

}
