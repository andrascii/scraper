#include "click_element_action.h"

namespace api {

ClickElementAction::ClickElementAction(const nlohmann::json& json)
  : OptionalAction{json} {
  xpath_ = json.at("xPath").get<std::string>();
}

ClickElementAction::ClickElementAction(bool optional, std::string xpath)
  : OptionalAction{optional},
    xpath_{std::move(xpath)} {}

nlohmann::json ClickElementAction::Serialize() const {
  nlohmann::json json;
  json["xPath"] = Xpath();
  json["type"] = ActionToString(ActionType::kClickElement);
  json.update(OptionalAction::Serialize());
  return json;
}

const std::string& ClickElementAction::Xpath() const noexcept {
  return xpath_;
}

void ClickElementAction::Accept(IActionVisitor* visitor) const {
  visitor->Visit(const_cast<ClickElementAction*>(this));
}

}