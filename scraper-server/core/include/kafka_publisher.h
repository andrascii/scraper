#pragma once

#include "imessage_publisher.h"
#include "settings.h"

namespace core {

class KafkaPublisher final : public IMessagePublisher {
 public:

  KafkaPublisher(const std::shared_ptr<Settings>& settings);

  std::error_code Publish(const std::string& message) noexcept override;

 private:

  static cppkafka::Configuration CreateConfiguration(const std::shared_ptr<Settings>& settings) noexcept;

 private:

  cppkafka::Producer producer_;
  std::shared_ptr<Settings> settings_;
};

}// namespace core