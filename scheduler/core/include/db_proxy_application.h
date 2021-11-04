#pragma once

#include "helpers.h"
#include "iapplication.h"
#include "ihttp_handler_registry.h"
#include "imessage_publisher.h"
#include "imigration_factory.h"
#include "pg_connection_pool.h"
#include "settings.h"

namespace core {

class Application : public IApplication {
 public:

  Application(std::shared_ptr<Settings> settings,
              // std::unique_ptr<IMessagePublisher> publisher,
              std::shared_ptr<IMigrationFactory> factory,
              std::shared_ptr<IHttpHandlerRegistry> http_handler_registry);

  std::error_code Start();
  void Stop() noexcept;

 private:

  std::error_code StartHttpServer();

 private:

  std::shared_ptr<Settings> settings_;
  std::unique_ptr<IMessagePublisher> publisher_;
  std::shared_ptr<IMigrationFactory> migration_factory_;
  std::shared_ptr<PgConnectionPool> pg_pool_;
  std::shared_ptr<IHttpHandlerRegistry> http_handler_registry_;
  boost::asio::io_context ctx_;
};

}// namespace core
