#include "get_element_by_css_selector_action.h"

namespace core {

GetElementByCssSelectorAction::GetElementByCssSelectorAction(const nlohmann::json& json)
    : OptionalAction(json) {
  selector_ = json.at("selector").get<std::string>();
  out_field_ = json.at("outField").get<std::string>();
}

GetElementByCssSelectorAction::GetElementByCssSelectorAction(
  bool optional,
  std::string selector,
  std::string out_field)
    : OptionalAction{optional},
      selector_{std::move(selector)},
      out_field_{std::move(out_field)} {}

nlohmann::json GetElementByCssSelectorAction::Serialize() const {
  nlohmann::json json;
  json["selector"] = selector_;
  json["outField"] = out_field_;
  json["type"] = ActionToString(Type::kGetElementByCssSelector);
  json.update(OptionalAction::Serialize());
  return json;
}

const std::string& GetElementByCssSelectorAction::Selector() const noexcept { return selector_; }

const std::string& GetElementByCssSelectorAction::OutField() const noexcept { return out_field_; }

}// namespace core
