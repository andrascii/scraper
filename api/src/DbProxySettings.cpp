#include "DbProxySettings.h"
#include "Errors.h"
#include "Exceptions.h"

namespace {

constexpr auto kDbOption{ "d,Database" };
constexpr auto kDbOptionDescription{ "The PostgreSQL Database name" };

constexpr auto kDbUserOption{ "u,dbuser" };
constexpr auto kDbUserOptionDescription{ "The PostgreSQL Database user" };

constexpr auto kDbUserPasswordOption{ "dbpass" };
constexpr auto kDbUserPasswordOptionDescription{ "The PostgreSQL Database password" };

constexpr auto kDbHostOption{ "h,host" };
constexpr auto kDbHostOptionDescription{ "The PostgreSQL Database host" };

constexpr auto kDbPortOption{ "dbport" };
constexpr auto kDbPortOptionDescription{ "The PostgreSQL Database port" };

constexpr auto kHttpPortOption{ "http-port" };
constexpr auto kHttpPortOptionDescription{ "The HTTP port to listen" };

constexpr auto kLogLevelOption{ "l,level" };
constexpr auto kLogLevelOptionDescription{ "Specifies log level [trace/debug/info/warning/error/critical]" };

constexpr auto kEnableConsoleOption{ "enable-console-logging" };
constexpr auto kEnableConsoleOptionDescription{ "Enables console logging" };

template <typename T>
T GetOptionValue(const cxxopts::ParseResult& result, const std::string& option) {
  const auto comma_position = option.find(',');
  const auto is_composed_option = comma_position != std::string::npos;

  if (!is_composed_option) {
    return result[option].as<T>();
  }

  try {
    return  result[option.substr(0, comma_position)].as<T>();
  } catch (const std::exception& ex) {
    return  result[option.substr(comma_position)].as<T>();
  }
}

}

namespace api {

common::Expected<SharedDbProxySettings, std::error_code>
DbProxySettings::Read(
  int argc,
  char** argv,
  const std::string& app_name,
  const std::string& app_description
) {
  try {
    cxxopts::Options options{app_name, app_description};

    options.add_options()
      (kDbOption, kDbOptionDescription, cxxopts::value<std::string>()->default_value("scraper"))
      (kDbUserOption, kDbUserOptionDescription, cxxopts::value<std::string>()->default_value("postgres"))
      (kDbUserPasswordOption, kDbUserPasswordOptionDescription, cxxopts::value<std::string>()->default_value("postgres"))
      (kDbHostOption, kDbHostOptionDescription, cxxopts::value<std::string>()->default_value("localhost"))
      (kDbPortOption, kDbPortOptionDescription, cxxopts::value<uint16_t>()->default_value("5432"))
      (kHttpPortOption, kHttpPortOptionDescription, cxxopts::value<uint16_t>()->default_value("13337"))
      (kLogLevelOption, kLogLevelOptionDescription, cxxopts::value<std::string>()->default_value("info"))
      ("help", "Prints this help message")
      (kEnableConsoleOption, kEnableConsoleOptionDescription);

    const auto result = options.parse(argc, argv);

    if (result.count("help") > 0) {
      throw HelpMessageRequested{ options.help() };
    }

    const auto has_option = [&result](const std::string &option) -> bool {
      const auto comma_position = option.find(',');
      const auto is_composed_option = comma_position != std::string::npos;

      if (!is_composed_option) {
        return result.count(option) > 0;
      }

      if (result.count(option.substr(0, comma_position)) > 0) {
        return true;
      }

      if (result.count(option.substr(comma_position)) > 0) {
        return true;
      }

      return false;
    };

    Settings settings{};
    settings.database = GetOptionValue<std::string>(result, kDbOption);
    settings.database_user = GetOptionValue<std::string>(result, kDbUserOption);
    settings.database_password = GetOptionValue<std::string>(result, kDbUserPasswordOption);
    settings.database_host = GetOptionValue<std::string>(result, kDbHostOption);
    settings.database_port = GetOptionValue<uint16_t>(result, kDbPortOption);
    settings.http_port = GetOptionValue<uint16_t>(result, kHttpPortOption);
    settings.enable_console_logging = has_option(kEnableConsoleOption);

    auto log_level = GetOptionValue<std::string>(result, kLogLevelOption);

    if (boost::iequals("trace", log_level)) {
      settings.log_level = spdlog::level::trace;
    } else if (boost::iequals("debug", log_level)) {
      settings.log_level = spdlog::level::debug;
    } else if (boost::iequals("info", log_level)) {
      settings.log_level = spdlog::level::info;
    } else if (boost::iequals("warning", log_level)) {
      settings.log_level = spdlog::level::warn;
    } else if (boost::iequals("error", log_level)) {
      settings.log_level = spdlog::level::err;
    } else if (boost::iequals("critical", log_level)) {
      settings.log_level = spdlog::level::critical;
    } else {
      throw std::runtime_error{"Invalid log level argument: " + log_level};
    }

    return std::shared_ptr<DbProxySettings>{ new DbProxySettings{ settings } };
  } catch (const HelpMessageRequested&) {
    throw;
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("Error initializing settings: {:s}", ex.what());
    return common::Unexpected<std::error_code>{ MakeErrorCode(DbProxyError::kCommandLineParsingError) };
  }
}

std::string DbProxySettings::Database() const noexcept {
  return settings_.database;
}

std::string DbProxySettings::DatabaseUser() const noexcept {
  return settings_.database_user;
}

std::string DbProxySettings::DatabasePassword() const noexcept {
  return settings_.database_password;
}

std::string DbProxySettings::DatabaseHost() const noexcept {
  return settings_.database_host;
}

uint16_t DbProxySettings::DatabasePort() const noexcept {
  return settings_.database_port;
}

uint16_t DbProxySettings::HttpPort() const noexcept {
  return settings_.http_port;
}

spdlog::level::level_enum DbProxySettings::LogLevel() const noexcept {
  return settings_.log_level;
}

bool DbProxySettings::EnableConsoleLogging() const noexcept {
  return settings_.enable_console_logging;
}

DbProxySettings::DbProxySettings(Settings settings)
  : settings_{ std::move( settings ) } {}

}
