#pragma once

#include "optional_action.h"

namespace api {

class ClickElementAction final : public OptionalAction {
 public:
  explicit ClickElementAction(const nlohmann::json& json);
  ClickElementAction(bool _optional, std::string xpath);

  void Accept(IActionVisitor* visitor) const override;
  [[nodiscard]] nlohmann::json Serialize() const override;
  [[nodiscard]] const std::string& Xpath() const noexcept;

 private:
  std::string xpath_;
};

}