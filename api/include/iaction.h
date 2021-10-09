#pragma once

#include "iaction_visitor.h"

namespace api {

class IAction {
 public:
  virtual ~IAction() = default;
  virtual void Accept(IActionVisitor* visitor) const = 0;
  [[nodiscard]] virtual nlohmann::json Serialize() const = 0;
};

}