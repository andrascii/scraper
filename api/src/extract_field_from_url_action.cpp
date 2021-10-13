#include "extract_field_from_url_action.h"

namespace api {

ExtractFieldFromUrlAction::ExtractFieldFromUrlAction(const nlohmann::json& json)
  : OptionalAction{json} {
  unique_id_ = json.at("uniqueId").get<bool>();
  reg_exp_ = json.at("regExp").get<std::string>();
  out_field_ = json.at("outField").get<std::string>();
}

ExtractFieldFromUrlAction::ExtractFieldFromUrlAction(
  bool optional,
  bool unique_id,
  std::string reg_exp,
  std::string out_field
) : OptionalAction{optional},
    unique_id_{unique_id},
    reg_exp_{std::move(reg_exp)},
    out_field_{std::move(out_field)} {}

nlohmann::json ExtractFieldFromUrlAction::Serialize() const {
  nlohmann::json json;
  json["uniqueId"] = unique_id_;
  json["regExp"] = reg_exp_;
  json["outField"] = out_field_;
  json["type"] = ActionToString(Type::kExtractFieldFromUrl);
  json.update(OptionalAction::Serialize());
  return json;
}

bool ExtractFieldFromUrlAction::UniqueId() const noexcept {
  return unique_id_;
}

const std::string& ExtractFieldFromUrlAction::RegExp() const noexcept {
  return reg_exp_;
}

const std::string& ExtractFieldFromUrlAction::OutField() const noexcept {
  return out_field_;
}

}