#pragma once

#include "iaction.h"

namespace api {

class IActionFactory {
 public:
  virtual ~IActionFactory() = default;

  virtual std::shared_ptr<IAction> Create(IAction::Type type, const nlohmann::json& json) const = 0;
};

}