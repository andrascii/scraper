#include "type_text_on_element_by_xpath_action.h"

namespace api {

TypeTextOnElementByXpathAction::TypeTextOnElementByXpathAction(const nlohmann::json& json)
  : OptionalAction(json) {
  text_ = json.at("text").get<std::string>();
  xpath_ = json.at("xPath").get<std::string>();
}

TypeTextOnElementByXpathAction::TypeTextOnElementByXpathAction(bool optional, std::string text, std::string xpath)
  : OptionalAction{optional},
    text_{std::move(text)},
    xpath_{std::move(xpath)} {}

nlohmann::json TypeTextOnElementByXpathAction::Serialize() const {
  nlohmann::json json;
  json["text"] = Text();
  json["xPath"] = Xpath();
  json.update(OptionalAction::Serialize());
  return json;
}

const std::string& TypeTextOnElementByXpathAction::Text() const {
  return text_;
}

const std::string& TypeTextOnElementByXpathAction::Xpath() const {
  return xpath_;
}

}