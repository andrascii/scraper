#include "KafkaPublisher.h"

namespace api {

KafkaPublisher::KafkaPublisher(const SharedSettings& settings)
  : producer_{ CreateConfiguration(settings) },
    settings_ { settings } {}

std::error_code KafkaPublisher::Publish(const std::string& message) noexcept {
  try {
    producer_.produce(cppkafka::MessageBuilder(settings_->KafkaOutputTopic()).partition(0).payload(message));
    producer_.flush();
    return std::error_code{};
  } catch (const std::exception& ex) {
    return std::make_error_code(std::errc::interrupted);
  }
}

cppkafka::Configuration KafkaPublisher::CreateConfiguration(const SharedSettings& settings) noexcept {
  return cppkafka::Configuration{
    { "metadata.broker.list", settings->KafkaBrokerList() }
  };
}

}