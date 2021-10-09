#pragma once

#include "iaction.h"

namespace api {

class SetCookiesAction final : public IAction {
 public:
  explicit SetCookiesAction(const nlohmann::json& json);
  SetCookiesAction(std::string name, std::string value);

  void Accept(IActionVisitor* visitor) const override;
  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] const std::string& Name() const;
  [[nodiscard]] const std::string& Value() const;

 private:
  std::string name_;
  std::string value_;
};

}