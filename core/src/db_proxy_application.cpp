#include "db_proxy_application.h"
#include "listener.h"
#include "helpers.h"
#include "errors.h"
#include "kafka_publisher.h"
#include "add_job_handler.h"
#include "remove_job_handler.h"
#include "enable_job_handler.h"
#include "disable_job_handler.h"
#include "update_job_handler.h"
#include "job_list_handler.h"

namespace core {

DbProxyApplication::DbProxyApplication(
  std::shared_ptr<Settings> settings,
  //std::unique_ptr<IMessagePublisher> publisher,
  std::shared_ptr<IMigrationFactory> factory,
  std::shared_ptr<IHttpHandlerRegistry> http_handler_registry
) : settings_{std::move(settings)},
  //publisher_{std::move(publisher)},
    migration_factory_{std::move(factory)},
    pg_pool_{std::make_shared<PgConnectionPool>(settings_, 32)},
    http_handler_registry_{std::move(http_handler_registry)},
    ctx_{std::max<int>(static_cast<int>(std::thread::hardware_concurrency()), 1)} {
  http_handler_registry_->AddPostHandler(
    PostHttpHandlerType::kAddJob,
    std::make_shared<AddJobHandler>(pg_pool_)
  );

  http_handler_registry_->AddPostHandler(
    PostHttpHandlerType::kRemoveJob,
    std::make_shared<RemoveJobHandler>(pg_pool_)
  );

  http_handler_registry_->AddPostHandler(
    PostHttpHandlerType::kEnableJob,
    std::make_shared<EnableJobHandler>(pg_pool_)
  );

  http_handler_registry_->AddPostHandler(
    PostHttpHandlerType::kDisableJob,
    std::make_shared<DisableJobHandler>(pg_pool_)
  );

  http_handler_registry_->AddPostHandler(
    PostHttpHandlerType::kUpdateJob,
    std::make_shared<UpdateJobHandler>(pg_pool_)
  );

  http_handler_registry_->AddPostHandler(
    PostHttpHandlerType::kJobList,
    std::make_shared<JobListHandler>(pg_pool_)
  );
}

std::error_code DbProxyApplication::Start() {
  const auto migration = migration_factory_->Create(IMigrationFactory::kScraper);

  if (!migration) {
    return migration.error();
  }

  const auto wrapper = pg_pool_->Take();
  (*migration)->ExecuteIfNeeded(wrapper.Get());

  return StartHttpServer();
}

void DbProxyApplication::Stop() noexcept {
  SPDLOG_INFO("Stopping application...");
  ctx_.stop();
}

std::error_code DbProxyApplication::StartHttpServer() {
  const auto thread_count = std::max<int>(static_cast<int>(std::thread::hardware_concurrency()), 1);
  const auto address = boost::asio::ip::make_address("0.0.0.0");
  const auto port = uint16_t{settings_->HttpPort()};

  std::make_shared<Listener>(
    ctx_,
    boost::asio::ip::tcp::endpoint{address, port},
    http_handler_registry_
  )->Run();

  std::vector<std::thread> threads;
  threads.reserve(thread_count - 1);

  for (auto i = thread_count - 1; i > 0; --i) {
    threads.emplace_back(
      [&, i] {
        common::Helpers::SetCurrentThreadName("listener #" + std::to_string(i));
        ctx_.run();
      }
    );
  }

  boost::system::error_code error;
  ctx_.run(error);

  return MakeErrorCode(error);
}

}
