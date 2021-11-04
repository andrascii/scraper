#pragma once

namespace core {

class IMessagePublisher {
 public:

  virtual ~IMessagePublisher() = default;

  virtual std::error_code Publish(const std::string& message) noexcept = 0;
};

}// namespace core