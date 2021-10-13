#include "key_press_action.h"

namespace api {

KeyPressAction::KeyPressAction(const nlohmann::json& json)
  : OptionalAction{json} {
  key_code_ = json.at("keyCode").get<uint64_t>();
}

KeyPressAction::KeyPressAction(bool optional, uint64_t key_code)
  : OptionalAction{optional},
    key_code_{key_code} {}

nlohmann::json KeyPressAction::Serialize() const {
  nlohmann::json json;
  json["keyCode"] = KeyCode();
  json["type"] = ActionToString(Type::kKeyPress);
  json.update(OptionalAction::Serialize());
  return json;
}

uint64_t KeyPressAction::KeyCode() const {
  return key_code_;
}

}