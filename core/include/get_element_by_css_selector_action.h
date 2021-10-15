#pragma once

#include "optional_action.h"

namespace core {

class GetElementByCssSelectorAction final : public OptionalAction {
 public:
  explicit GetElementByCssSelectorAction(const nlohmann::json& json);

  GetElementByCssSelectorAction(
    bool optional,
    bool unique,
    std::string selector,
    std::string out_field
  );

  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] bool UniqueId() const noexcept;
  [[nodiscard]] const std::string& Selector() const noexcept;
  [[nodiscard]] const std::string& OutField() const noexcept;

 private:
  bool unique_id_;
  std::string selector_;
  std::string out_field_;
};

}