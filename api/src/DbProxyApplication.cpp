#include "DbProxyApplication.h"
#include "Logger.h"
#include "Errors.h"
#include "Listener.h"

namespace api {

DbProxyApplication::DbProxyApplication(DbProxySettings settings)
  :  settings_{ std::move( settings ) } {}

std::error_code DbProxyApplication::Start() noexcept {
  try {
    const auto thread_count = std::max<int>(static_cast<int>(std::thread::hardware_concurrency()), 1);
    const auto address = boost::asio::ip::make_address("0.0.0.0");
    const auto port = uint16_t{13337};

    auto ctx = boost::asio::io_context{thread_count};
    std::make_shared<Listener>(ctx, boost::asio::ip::tcp::endpoint{address, port})->Run();

    std::vector<std::thread> threads;
    threads.reserve(thread_count - 1);

    for (auto i = thread_count - 1; i > 0; --i) {
      threads.emplace_back([&ctx] { ctx.run(); });
    }

    ctx.run();
    return {};
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("Something bad: {:s}", ex.what());
    return std::make_error_code(std::errc::operation_canceled);
  }
}

}
