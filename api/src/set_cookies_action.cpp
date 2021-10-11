#include "set_cookies_action.h"

namespace api {

SetCookiesAction::SetCookiesAction(const nlohmann::json& json) {
  name_ = json.at("name").get<std::string>();
  value_ = json.at("value").get<std::string>();
}

SetCookiesAction::SetCookiesAction(std::string name, std::string value)
  : name_{std::move(name)},
    value_{std::move(value)} {}

nlohmann::json SetCookiesAction::Serialize() const {
  nlohmann::json json;
  json["name"] = Name();
  json["value"] = Value();
  json["type"] = ActionToString(ActionType::kSetCookies);
  return json;
}

const std::string& SetCookiesAction::Name() const {
  return name_;
}

const std::string& SetCookiesAction::Value() const {
  return value_;
}

void SetCookiesAction::Accept(IActionVisitor* visitor) const {
  visitor->Visit(const_cast<SetCookiesAction*>(this));
}

}