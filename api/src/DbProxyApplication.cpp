#include "DbProxyApplication.h"
#include "Logger.h"
#include "Listener.h"
#include "Helpers.h"
#include "KafkaPublisher.h"

namespace api {

DbProxyApplication::DbProxyApplication(
  SharedSettings settings,
  std::unique_ptr<IMessagePublisher> publisher)
  : settings_{std::move(settings)},
    publisher_{std::move(publisher)} {}

std::error_code DbProxyApplication::Start() noexcept {
  try {
    const auto thread_count = std::max<int>(static_cast<int>(std::thread::hardware_concurrency()), 1);
    const auto address = boost::asio::ip::make_address("0.0.0.0");
    const auto port = uint16_t{ settings_->HttpPort() };

    auto ctx = boost::asio::io_context{thread_count};
    std::make_shared<Listener>(ctx, boost::asio::ip::tcp::endpoint{address, port})->Run();

    std::vector<std::thread> threads;
    threads.reserve(thread_count - 1);

    for (auto i = thread_count - 1; i > 0; --i) {
      threads.emplace_back([&ctx, i] {
        common::Helpers::SetCurrentThreadName("listener #" + std::to_string(i));
        ctx.run();
      });
    }

    ctx.run();
    return {};
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("Something bad: {:s}", ex.what());
    return std::make_error_code(std::errc::operation_canceled);
  }
}

}
