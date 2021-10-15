#include "set_cookies_action.h"

namespace core {

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
  json["type"] = ActionToString(Type::kSetCookies);
  return json;
}

const std::string& SetCookiesAction::Name() const {
  return name_;
}

const std::string& SetCookiesAction::Value() const {
  return value_;
}

}