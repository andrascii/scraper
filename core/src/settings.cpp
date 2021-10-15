#include "settings.h"
#include "errors.h"
#include "exceptions.h"

namespace {

constexpr auto kBrokerListOption{"b,brokers"};
constexpr auto kBrokerListOptionDescription{"The Kafka server address"};

constexpr auto kKafkaInputTopicOption{"i,kafka-input-topic"};
constexpr auto kKafkaInputTopicOptionDescription{"The PostgreSQL Database name"};

constexpr auto kKafkaOutputTopicOption{"o,kafka-output-topic"};
constexpr auto kKafkaOutputTopicOptionDescription{"The PostgreSQL Database name"};

constexpr auto kDbOption{"d,database"};
constexpr auto kDbOptionDescription{"The PostgreSQL Database name"};

constexpr auto kDbUserOption{"u,dbuser"};
constexpr auto kDbUserOptionDescription{"The PostgreSQL Database user"};

constexpr auto kDbUserPasswordOption{"dbpass"};
constexpr auto kDbUserPasswordOptionDescription{"The PostgreSQL Database password"};

constexpr auto kDbHostOption{"h,host"};
constexpr auto kDbHostOptionDescription{"The PostgreSQL Database host"};

constexpr auto kDbPortOption{"dbport"};
constexpr auto kDbPortOptionDescription{"The PostgreSQL Database port"};

constexpr auto kHttpPortOption{"http-port"};
constexpr auto kHttpPortOptionDescription{"The HTTP port to listen"};

constexpr auto kLogLevelOption{"l,level"};
constexpr auto kLogLevelOptionDescription{"Specifies log level [trace/debug/info/warning/error/critical]"};

constexpr auto kEnableConsoleOption{"enable-console-logging"};
constexpr auto kEnableConsoleOptionDescription{"Enables console logging"};

template <typename T>
T GetOptionValue(const cxxopts::ParseResult& result, const std::string& option) {
  const auto comma_position = option.find(',');
  const auto is_composed_option = comma_position != std::string::npos;

  if (!is_composed_option) {
    return result[option].as<T>();
  }

  try {
    return result[option.substr(0, comma_position)].as<T>();
  } catch (const std::exception& ex) {
    return result[option.substr(comma_position)].as<T>();
  }
}

}

namespace core {

tl::expected<std::shared_ptr<Settings>, std::error_code> Settings::Read(
  int argc,
  char **argv,
  const std::string& app_name,
  const std::string& app_description
) {
  try {
    cxxopts::Options options{app_name, app_description};

    options.add_options()
      (kBrokerListOption, kBrokerListOptionDescription, cxxopts::value<std::string>()->default_value("localhost:9092"))
      (kKafkaInputTopicOption, kKafkaInputTopicOptionDescription, cxxopts::value<std::string>()->default_value("crawl-page-result"))
      (kKafkaOutputTopicOption, kKafkaOutputTopicOptionDescription, cxxopts::value<std::string>()->default_value("crawl-queue"))
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
      throw HelpMessageRequested{options.help()};
    }

    const auto has_option = [&result](const std::string& option) -> bool {
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

    Data data{};
    data.kafka_broker_list = GetOptionValue<std::string>(result, kBrokerListOption);
    data.kafka_input_topic = GetOptionValue<std::string>(result, kKafkaInputTopicOption);
    data.kafka_output_topic = GetOptionValue<std::string>(result, kKafkaOutputTopicOption);
    data.database = GetOptionValue<std::string>(result, kDbOption);
    data.database_user = GetOptionValue<std::string>(result, kDbUserOption);
    data.database_password = GetOptionValue<std::string>(result, kDbUserPasswordOption);
    data.database_host = GetOptionValue<std::string>(result, kDbHostOption);
    data.database_port = GetOptionValue<uint16_t>(result, kDbPortOption);
    data.http_port = GetOptionValue<uint16_t>(result, kHttpPortOption);
    data.enable_console_logging = has_option(kEnableConsoleOption);

    auto log_level = GetOptionValue<std::string>(result, kLogLevelOption);

    if (boost::iequals("trace", log_level)) {
      data.log_level = spdlog::level::trace;
    } else if (boost::iequals("debug", log_level)) {
      data.log_level = spdlog::level::debug;
    } else if (boost::iequals("info", log_level)) {
      data.log_level = spdlog::level::info;
    } else if (boost::iequals("warning", log_level)) {
      data.log_level = spdlog::level::warn;
    } else if (boost::iequals("error", log_level)) {
      data.log_level = spdlog::level::err;
    } else if (boost::iequals("critical", log_level)) {
      data.log_level = spdlog::level::critical;
    } else {
      throw std::runtime_error{"Invalid log level argument: " + log_level};
    }

    return std::shared_ptr<Settings>{new Settings{data}};
  } catch (const HelpMessageRequested&) {
    throw;
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("Error initializing settings: {:s}", ex.what());
    return tl::unexpected<std::error_code>{MakeErrorCode(Error::kCommandLineParsingError)};
  }
}

std::string Settings::KafkaBrokerList() const noexcept {
  return data_.kafka_broker_list;
}

std::string Settings::KafkaInputTopic() const noexcept {
  return data_.kafka_input_topic;
}

std::string Settings::KafkaOutputTopic() const noexcept {
  return data_.kafka_output_topic;
}

std::string Settings::Database() const noexcept {
  return data_.database;
}

std::string Settings::DatabaseUser() const noexcept {
  return data_.database_user;
}

std::string Settings::DatabasePassword() const noexcept {
  return data_.database_password;
}

std::string Settings::DatabaseHost() const noexcept {
  return data_.database_host;
}

uint16_t Settings::DatabasePort() const noexcept {
  return data_.database_port;
}

uint16_t Settings::HttpPort() const noexcept {
  return data_.http_port;
}

spdlog::level::level_enum Settings::LogLevel() const noexcept {
  return data_.log_level;
}

bool Settings::EnableConsoleLogging() const noexcept {
  return data_.enable_console_logging;
}

Settings::Settings(Data data)
  : data_{std::move(data)} {}

}
