#pragma once

#include "iaction.h"

namespace core {

class OptionalAction : public IAction {
 public:

  explicit OptionalAction(bool is_optional);
  explicit OptionalAction(const nlohmann::json& json);

  [[nodiscard]] nlohmann::json Serialize() const override;

  bool IsOptional() const noexcept;

 private:

  bool is_optional_;
};

}// namespace core