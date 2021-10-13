#include "run_script_action.h"

namespace api {

RunScriptAction::RunScriptAction(const nlohmann::json& json)
  : OptionalAction(json) {
  script_ = json.at("script").get<std::string>();
}

RunScriptAction::RunScriptAction(bool optional, std::string script)
  : OptionalAction{optional},
    script_{std::move(script)} {}

nlohmann::json RunScriptAction::Serialize() const {
  nlohmann::json json;
  json["script"] = Script();
  json["type"] = ActionToString(Type::kRunScript);
  json.update(OptionalAction::Serialize());
  return json;
}

const std::string& RunScriptAction::Script() const {
  return script_;
}

}