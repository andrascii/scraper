#pragma once

#include "optional_action.h"

namespace core {

class KeyPressAction final : public OptionalAction {
 public:

  explicit KeyPressAction(const nlohmann::json& json);
  KeyPressAction(bool optional, uint64_t key_code);

  [[nodiscard]] nlohmann::json Serialize() const override;
  [[nodiscard]] uint64_t KeyCode() const;

 private:

  uint64_t key_code_;
};

}// namespace core