#include "emulate_mouse_move_action.h"

namespace api {

EmulateMouseMoveAction::EmulateMouseMoveAction(const nlohmann::json &json)
  : OptionalAction{json} {}

EmulateMouseMoveAction::EmulateMouseMoveAction(bool is_optional)
  : OptionalAction{is_optional} {}

void EmulateMouseMoveAction::Accept(IActionVisitor* visitor) const {
  visitor->Visit(const_cast<EmulateMouseMoveAction*>(this));
}

}