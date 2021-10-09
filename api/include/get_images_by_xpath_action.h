#pragma once

#include "optional_action.h"

namespace api {

class GetImagesByXpathAction final : public OptionalAction {
 public:
  explicit GetImagesByXpathAction(const nlohmann::json& json);
  GetImagesByXpathAction(bool optional, std::string xpath, std::string out_field);

  void Accept(IActionVisitor* visitor) const override;
  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] const std::string& Xpath() const;
  [[nodiscard]] const std::string& OutField() const;

 private:
  std::string xpath_;
  std::string out_field_;
};

}