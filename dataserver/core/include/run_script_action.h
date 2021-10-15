#pragma once

#include "optional_action.h"

namespace core {

class RunScriptAction final : public OptionalAction {
 public:
  explicit RunScriptAction(const nlohmann::json& json);
  RunScriptAction(bool optional, std::string script);

  [[nodiscard]] nlohmann::json Serialize() const override;
  [[nodiscard]] const std::string& Script() const;

 private:
  std::string script_;
};

}