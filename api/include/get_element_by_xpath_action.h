#pragma once

#include <nlohmann/json.hpp>
#include "optional_action.h"

namespace api {

class GetElementByXpathAction final : public OptionalAction {
 public:
  explicit GetElementByXpathAction(const nlohmann::json& json);

  GetElementByXpathAction(
    bool optional,
    bool unique,
    std::string xpath,
    std::string out_field
  );

  [[nodiscard]] nlohmann::json Serialize() const override;

  [[nodiscard]] bool UniqueId() const noexcept;
  [[nodiscard]] const std::string& Xpath() const noexcept;
  [[nodiscard]] const std::string& OutField() const noexcept;

 private:
  bool unique_id_;
  std::string xpath_;
  std::string out_field_;
};

}