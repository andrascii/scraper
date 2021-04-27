#pragma once

#include "Helpers.h"

namespace api {

class DbProxySettings final {
public:
  static common::Expected<DbProxySettings, std::error_code> read(
    int argc,
    char** argv,
    const std::string& app_name,
    const std::string& app_description
  ) noexcept;

  std::string database() const noexcept;
  std::string databaseHost() const noexcept;
  uint16_t databasePort() const noexcept;
  spdlog::level::level_enum logLevel() const noexcept;
  bool enableConsoleLogging() const noexcept;

private:
  struct Settings {
    std::string database;
    std::string database_host;
    uint16_t database_port;
    bool enable_console_logging{ false };
    spdlog::level::level_enum log_level{ spdlog::level::info };
  };

  DbProxySettings(const Settings& settings);

private:
  Settings settings_;
};

}
