#include "get_images_by_xpath_action.h"

namespace api {

GetImagesByXpathAction::GetImagesByXpathAction(const nlohmann::json& json)
  : OptionalAction{json} {
  xpath_ = json.at("xPath").get<std::string>();
  out_field_ = json.at("outField").get<std::string>();
}

GetImagesByXpathAction::GetImagesByXpathAction(bool optional, std::string xpath, std::string out_field)
  : OptionalAction{optional},
    xpath_{std::move(xpath)},
    out_field_{std::move(out_field)} {}

nlohmann::json GetImagesByXpathAction::Serialize() const {
  nlohmann::json json;
  json["xPath"] = xpath_;
  json["outField"] = out_field_;
  json["type"] = ActionToString(ActionType::kGetImagesByXpath);
  json.update(OptionalAction::Serialize());
  return json;
}

const std::string& GetImagesByXpathAction::Xpath() const {
  return xpath_;
}

const std::string& GetImagesByXpathAction::OutField() const {
  return out_field_;
}

void GetImagesByXpathAction::Accept(IActionVisitor* visitor) const {
  visitor->Visit(const_cast<GetImagesByXpathAction*>(this));
}

}