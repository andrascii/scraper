#pragma once

#include "optional_action.h"

namespace api {

class ExtractFieldFromUrlAction final : public OptionalAction {
 public:
  explicit ExtractFieldFromUrlAction(const nlohmann::json& json);

  ExtractFieldFromUrlAction(
    bool optional,
    bool unique_id,
    std::string reg_exp,
    std::string out_field
  );

  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] bool UniqueId() const noexcept;
  [[nodiscard]] const std::string& RegExp() const noexcept;
  [[nodiscard]] const std::string& OutField() const noexcept;

 private:
  bool unique_id_;
  std::string reg_exp_;
  std::string out_field_;
};

}