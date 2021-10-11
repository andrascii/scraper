#pragma once

#include "optional_action.h"

namespace api {

class EmulateMouseMoveAction final : public OptionalAction {
 public:
  explicit EmulateMouseMoveAction(const nlohmann::json& json);
  explicit EmulateMouseMoveAction(bool is_optional);

  nlohmann::json Serialize() const override;
  void Accept(IActionVisitor* visitor) const override;
};

}