#include "go_to_url_action.h"

namespace api {

GoToUrlAction::GoToUrlAction(const nlohmann::json& json) {
  url_ = json.at("url").get<std::string>();
}

GoToUrlAction::GoToUrlAction(std::string url)
  : url_{std::move(url)} {}

nlohmann::json GoToUrlAction::Serialize() const {
  nlohmann::json json;
  json["url"] = Url();
  return json;
}

const std::string& GoToUrlAction::Url() const {
  return url_;
}

}