#pragma once

#include "iaction_factory.h"

namespace core {

class ActionFactory final : public IActionFactory {
 public:

  std::shared_ptr<IAction> Create(IAction::Type type, const nlohmann::json& json) const override;
};

}// namespace core