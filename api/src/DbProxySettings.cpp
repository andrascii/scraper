#include "DbProxySettings.h"
#include "Errors.h"

namespace {

auto constexpr kDbOption{ "d,database" };
auto constexpr kDbOptionDescription{ "The PostgreSQL database name" };

auto constexpr kDbHostOption{ "h,host" };
auto constexpr kDbHostOptionDescription{ "The PostgreSQL database host" };

auto constexpr kDbPortOption{ "p,port" };
auto constexpr kDbPortOptionDescription{ "The PostgreSQL database port" };

auto constexpr kLogLevelOption{ "l,level" };
auto constexpr kLogLevelOptionDescription{ "Specifies log level [trace/debug/info/warning/error/critical]" };

auto constexpr kEnableConsoleOption{ "enable-console-logging" };
auto constexpr kEnableConsoleOptionDescription{ "Enables console logging" };

template <typename T>
T GetOptionValue(const cxxopts::ParseResult& result, const std::string& option) {
  const auto comma_position = option.find(",");
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

common::Expected<DbProxySettings, std::error_code>
DbProxySettings::read(
  int argc,
  char** argv,
  const std::string& app_name,
  const std::string& app_description
) noexcept {
  try {
    cxxopts::Options options{ app_name, app_description };

    options.add_options()
      (kDbOption, kDbOptionDescription, cxxopts::value<std::string>()->default_value("scraper"))
      (kDbHostOption, kDbHostOptionDescription, cxxopts::value<std::string>()->default_value("localhost"))
      (kDbPortOption, kDbPortOptionDescription, cxxopts::value<uint16_t>()->default_value("5432"))
      (kLogLevelOption, kLogLevelOptionDescription, cxxopts::value<std::string>()->default_value("info"))
      (kEnableConsoleOption, kEnableConsoleOptionDescription);

    const auto result = options.parse(argc, argv);

    const auto has_option = [&result](std::string const& option) -> bool {
      const auto comma_position = option.find(",");
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
    settings.database_host = GetOptionValue<std::string>(result, kDbHostOption);
    settings.database_port = GetOptionValue<uint16_t>(result, kDbPortOption);
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
      throw std::runtime_error{ "Invalid log level argument: " + log_level };
    }

    return DbProxySettings{ settings };
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("Error initializing settings: {:s}", ex.what());
    return common::Unexpected<std::error_code>{ MakeErrorCode(DbProxyError::kCommandLineParsingError) };
  }
}

std::string DbProxySettings::database() const noexcept {
  return settings_.database;
}

std::string DbProxySettings::databaseHost() const noexcept {
  return settings_.database_host;
}

uint16_t DbProxySettings::databasePort() const noexcept {
  return settings_.database_port;
}

spdlog::level::level_enum DbProxySettings::logLevel() const noexcept {
  return settings_.log_level;
}

bool DbProxySettings::enableConsoleLogging() const noexcept {
  return settings_.enable_console_logging;
}

DbProxySettings::DbProxySettings(const Settings& settings) : settings_{ settings } {}

}
