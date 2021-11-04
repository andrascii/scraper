#pragma once

#include "optional_action.h"

namespace core {

class ExtractFieldFromUrlAction final : public OptionalAction {
 public:

  explicit ExtractFieldFromUrlAction(const nlohmann::json& json);

  ExtractFieldFromUrlAction(bool optional, std::string reg_exp, std::string out_field);

  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] const std::string& RegExp() const noexcept;
  [[nodiscard]] const std::string& OutField() const noexcept;

 private:

  std::string reg_exp_;
  std::string out_field_;
};

}// namespace core