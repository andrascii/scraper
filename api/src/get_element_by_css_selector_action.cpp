#include "get_element_by_css_selector_action.h"

namespace api {

GetElementByCssSelectorAction::GetElementByCssSelectorAction(const nlohmann::json& json)
  : OptionalAction(json) {
  unique_id_ = json.at("uniqueId").get<bool>();
  selector_ = json.at("selector").get<std::string>();
  out_field_ = json.at("outField").get<std::string>();
}

GetElementByCssSelectorAction::GetElementByCssSelectorAction(
  bool optional,
  bool unique,
  std::string selector,
  std::string out_field
) : OptionalAction{optional}
    , unique_id_{unique}
    , selector_{std::move(selector)}
    , out_field_{std::move(out_field)} {}

nlohmann::json GetElementByCssSelectorAction::Serialize() const {
  nlohmann::json json;
  json["uniqueId"] = unique_id_;
  json["selector"] = selector_;
  json["outField"] = out_field_;
  json.update(OptionalAction::Serialize());
  return json;
}

bool GetElementByCssSelectorAction::UniqueId() const noexcept {
  return unique_id_;
}

const std::string& GetElementByCssSelectorAction::Selector() const noexcept {
  return selector_;
}

const std::string& GetElementByCssSelectorAction::OutField() const noexcept {
  return out_field_;
}

}// namespace api