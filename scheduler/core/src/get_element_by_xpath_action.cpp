#include "get_element_by_xpath_action.h"

namespace core {

GetElementByXpathAction::GetElementByXpathAction(const nlohmann::json& json) : OptionalAction(json) {
  xpath_ = json.at("xPath").get<std::string>();
  out_field_ = json.at("outField").get<std::string>();
}

GetElementByXpathAction::GetElementByXpathAction(bool optional, std::string xpath, std::string out_field)
    : OptionalAction{optional},
      xpath_{std::move(xpath)},
      out_field_{std::move(out_field)} {}

nlohmann::json GetElementByXpathAction::Serialize() const {
  nlohmann::json json;
  json["xPath"] = xpath_;
  json["outField"] = out_field_;
  json["type"] = ActionToString(Type::kGetElementByXpath);
  json.update(OptionalAction::Serialize());
  return json;
}

const std::string& GetElementByXpathAction::Xpath() const noexcept { return xpath_; }

const std::string& GetElementByXpathAction::OutField() const noexcept { return out_field_; }

}// namespace core