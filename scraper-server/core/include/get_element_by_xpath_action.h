#pragma once

#include <nlohmann/json.hpp>

#include "optional_action.h"

namespace core {

class GetElementByXpathAction final : public OptionalAction {
 public:

  explicit GetElementByXpathAction(const nlohmann::json& json);

  GetElementByXpathAction(bool optional, std::string xpath, std::string out_field);

  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] const std::string& Xpath() const noexcept;
  [[nodiscard]] const std::string& OutField() const noexcept;

 private:

  std::string xpath_;
  std::string out_field_;
};

}// namespace core