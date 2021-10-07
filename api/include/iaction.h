#pragma once

namespace api {

class IAction {
 public:
  virtual ~IAction() = default;
  [[nodiscard]] virtual nlohmann::json Serialize() const = 0;
};

}