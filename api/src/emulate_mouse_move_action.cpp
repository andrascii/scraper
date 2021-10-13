#include "emulate_mouse_move_action.h"

namespace api {

EmulateMouseMoveAction::EmulateMouseMoveAction(const nlohmann::json &json)
  : OptionalAction{json} {}

EmulateMouseMoveAction::EmulateMouseMoveAction(bool is_optional)
  : OptionalAction{is_optional} {}

nlohmann::json EmulateMouseMoveAction::Serialize() const {
  nlohmann::json json;
  json["type"] = ActionToString(Type::kEmulateMouseMove);
  json.update(OptionalAction::Serialize());
  return json;
}

}