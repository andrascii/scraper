#pragma once

#include "Helpers.h"

namespace api {

class DbProxySettings;
using SharedDbProxySettings = std::shared_ptr<DbProxySettings>;

class DbProxySettings final {
public:
  static common::Expected<SharedDbProxySettings, std::error_code> Read(
    int argc,
    char** argv,
    const std::string& app_name,
    const std::string& app_description
  );

  //
  // DB settings
  //
  [[nodiscard]] std::string Database() const noexcept;
  [[nodiscard]] std::string DatabaseUser() const noexcept;
  [[nodiscard]] std::string DatabasePassword() const noexcept;
  [[nodiscard]] std::string DatabaseHost() const noexcept;
  [[nodiscard]] uint16_t DatabasePort() const noexcept;

  //
  // HTTP server settings
  //
  [[nodiscard]] uint16_t HttpPort() const noexcept;

  //
  // Logging
  //
  [[nodiscard]] spdlog::level::level_enum LogLevel() const noexcept;
  [[nodiscard]] bool EnableConsoleLogging() const noexcept;

private:
  struct Settings {
    std::string database;
    std::string database_user;
    std::string database_password;
    std::string database_host;
    uint16_t database_port;
    uint16_t http_port;
    bool enable_console_logging{ false };
    spdlog::level::level_enum log_level{ spdlog::level::info };
  };

  explicit DbProxySettings(Settings settings);

private:
  Settings settings_;
};

}
