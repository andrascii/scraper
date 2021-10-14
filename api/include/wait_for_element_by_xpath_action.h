#pragma once

#include "iaction.h"

namespace api {

class WaitForElementByXpathAction final : public IAction {
 public:
  using Ms = std::chrono::milliseconds;

  explicit WaitForElementByXpathAction(const nlohmann::json& json);
  WaitForElementByXpathAction(std::string xpath, Ms timeout);

  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] const std::string& Xpath() const;
  [[nodiscard]] const Ms& Timeout() const;

 private:
  std::string xpath_;
  Ms timeout_{};
};

}