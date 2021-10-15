#pragma once

#include "helpers.h"

namespace core {

class Settings final {
 public:
  static tl::expected<std::shared_ptr<Settings>, std::error_code> Read(
    int argc,
    char** argv,
    const std::string& app_name,
    const std::string& app_description
  );

  //
  // Kafka settings
  //
  [[nodiscard]] std::string KafkaBrokerList() const noexcept;
  [[nodiscard]] std::string KafkaInputTopic() const noexcept;
  [[nodiscard]] std::string KafkaOutputTopic() const noexcept;

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
  struct Data {
    std::string kafka_broker_list;
    std::string kafka_input_topic;
    std::string kafka_output_topic;
    std::string database;
    std::string database_user;
    std::string database_password;
    std::string database_host;
    uint16_t database_port;
    uint16_t http_port;
    bool enable_console_logging{ false };
    spdlog::level::level_enum log_level{ spdlog::level::info };
  };

  explicit Settings(Data data);

 private:
  Data data_;
};

}
