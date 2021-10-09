#pragma once

#include "optional_action.h"

namespace api {

class TypeTextOnElementByXpathAction final : public OptionalAction {
 public:
  explicit TypeTextOnElementByXpathAction(const nlohmann::json& json);
  TypeTextOnElementByXpathAction(bool optional, std::string text, std::string xpath);

  void Accept(IActionVisitor* visitor) const override;
  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] const std::string& Text() const;
  [[nodiscard]] const std::string& Xpath() const;

 private:
  std::string text_;
  std::string xpath_;
};

}