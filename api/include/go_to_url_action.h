#include "iaction.h"

namespace api {

class GoToUrlAction final : public IAction {
 public:
  explicit GoToUrlAction(const nlohmann::json& json);
  explicit GoToUrlAction(std::string url);

  void Accept(IActionVisitor* visitor) const override;
  [[nodiscard]] nlohmann::json Serialize() const override;
  [[nodiscard]] const std::string& Url() const;

 private:
  std::string url_;
};

}