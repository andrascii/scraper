#include "iaction.h"

namespace core {

class GoToUrlAction final : public IAction {
 public:

  explicit GoToUrlAction(const nlohmann::json& json);
  explicit GoToUrlAction(std::string url);

  [[nodiscard]] nlohmann::json Serialize() const override;
  [[nodiscard]] const std::string& Url() const;

 private:

  std::string url_;
};

}// namespace core