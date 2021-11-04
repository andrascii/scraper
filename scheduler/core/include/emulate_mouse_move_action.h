#pragma once

#include "optional_action.h"

namespace core {

class EmulateMouseMoveAction final : public OptionalAction {
 public:

  explicit EmulateMouseMoveAction(const nlohmann::json& json);
  explicit EmulateMouseMoveAction(bool is_optional);

  nlohmann::json Serialize() const override;
};

}// namespace core