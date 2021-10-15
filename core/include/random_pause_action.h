#pragma once

#include "optional_action.h"

namespace core {

class RandomPauseAction final : public OptionalAction {
 public:
  using Ms = std::chrono::milliseconds;

  explicit RandomPauseAction(const nlohmann::json& json);
  RandomPauseAction(bool optional, const Ms& from, const Ms& to);

  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] const Ms& FromMs() const noexcept;
  [[nodiscard]] const Ms& ToMs() const noexcept;

 private:
  Ms from_;
  Ms to_;
};

}