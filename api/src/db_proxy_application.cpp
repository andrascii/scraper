#include "db_proxy_application.h"
#include "listener.h"
#include "helpers.h"
#include "errors.h"
#include "kafka_publisher.h"

namespace api {

DbProxyApplication::DbProxyApplication(
  SharedSettings settings,
  //std::unique_ptr<IMessagePublisher> publisher,
  std::vector<std::unique_ptr<IMigrator>> migrators)
  : settings_{std::move(settings)},
    //publisher_{std::move(publisher)},
    migrators_{std::move(migrators)},
    ctx_{std::max<int>(static_cast<int>(std::thread::hardware_concurrency()), 1)} {}

std::error_code DbProxyApplication::Start() {
  for (const auto& migrator : migrators_) {
    migrator->Apply();
  }

  return StartHttpServer();
}

void DbProxyApplication::Stop() noexcept {
  SPDLOG_INFO("Stopping application...");
  ctx_.stop();
}

std::error_code DbProxyApplication::StartHttpServer() {
  const auto thread_count = std::max<int>(static_cast<int>(std::thread::hardware_concurrency()), 1);
  const auto address = boost::asio::ip::make_address("0.0.0.0");
  const auto port = uint16_t{ settings_->HttpPort() };

  std::make_shared<Listener>(ctx_, boost::asio::ip::tcp::endpoint{address, port})->Run();

  std::vector<std::thread> threads;
  threads.reserve(thread_count - 1);

  for (auto i = thread_count - 1; i > 0; --i) {
    threads.emplace_back([&, i] {
      common::Helpers::SetCurrentThreadName("listener #" + std::to_string(i));
      ctx_.run();
    });
  }

  boost::system::error_code error;
  ctx_.run(error);

  return MakeErrorCode(error);
}

}