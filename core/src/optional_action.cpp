#include "optional_action.h"

namespace core {

OptionalAction::OptionalAction(bool is_optional)
  : is_optional_{is_optional} {}

OptionalAction::OptionalAction(const nlohmann::json& json) {
  is_optional_ = json.at("optional").get<bool>();
}

nlohmann::json OptionalAction::Serialize() const {
  nlohmann::json json;
  json["optional"] = is_optional_;
  return json;
}

bool OptionalAction::IsOptional() const noexcept {
  return is_optional_;
}

}