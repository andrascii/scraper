#pragma once

#include "iaction.h"

namespace core {

class SetCookiesAction final : public IAction {
 public:

  explicit SetCookiesAction(const nlohmann::json& json);
  SetCookiesAction(std::string name, std::string value);

  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] const std::string& Name() const;
  [[nodiscard]] const std::string& Value() const;

 private:

  std::string name_;
  std::string value_;
};

}// namespace core