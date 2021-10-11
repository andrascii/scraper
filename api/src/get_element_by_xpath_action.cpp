#include "get_element_by_xpath_action.h"

namespace api {

GetElementByXpathAction::GetElementByXpathAction(const nlohmann::json& json)
  : OptionalAction(json) {
  unique_id_ = json.at("uniqueId").get<bool>();
  xpath_ = json.at("xPath").get<std::string>();
  out_field_ = json.at("outField").get<std::string>();
}

GetElementByXpathAction::GetElementByXpathAction(bool optional, bool unique, std::string xpath, std::string out_field)
  : OptionalAction{optional},
    unique_id_{unique},
    xpath_{std::move(xpath)},
    out_field_{std::move(out_field)} {}

nlohmann::json GetElementByXpathAction::Serialize() const {
  nlohmann::json json;
  json["uniqueId"] = unique_id_;
  json["xPath"] = xpath_;
  json["outField"] = out_field_;
  json["type"] = ActionToString(ActionType::kGetElementByXpath);
  json.update(OptionalAction::Serialize());
  return json;
}

bool GetElementByXpathAction::UniqueId() const noexcept {
  return unique_id_;
}

const std::string& GetElementByXpathAction::Xpath() const noexcept {
  return xpath_;
}

const std::string& GetElementByXpathAction::OutField() const noexcept {
  return out_field_;
}

void GetElementByXpathAction::Accept(IActionVisitor* visitor) const {
  visitor->Visit(const_cast<GetElementByXpathAction*>(this));
}

}