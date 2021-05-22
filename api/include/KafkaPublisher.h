#pragma once

#include "IMessagePublisher.h"
#include "Settings.h"

namespace api {

class KafkaPublisher final : public IMessagePublisher {
public:
  KafkaPublisher(const SharedSettings& settings);

  std::error_code Publish(const std::string& message) noexcept override;

private:
  static cppkafka::Configuration CreateConfiguration(const SharedSettings& settings) noexcept;

private:
  cppkafka::Producer producer_;
  SharedSettings settings_;
};

}