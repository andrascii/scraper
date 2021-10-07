#include "wait_for_element_action.h"

namespace api {

WaitForElementAction::WaitForElementAction(const nlohmann::json& json) {
  xpath_ = json.at("xPath").get<std::string>();
  timeout_ = Ms{json.at("maxTimeoutMsecs").get<uint64_t>()};
}

WaitForElementAction::WaitForElementAction(std::string xpath, WaitForElementAction::Ms timeout)
  : xpath_{std::move(xpath)},
    timeout_{std::move(timeout)} {}

nlohmann::json WaitForElementAction::Serialize() const {
  nlohmann::json json;
  json["xPath"] = Xpath();
  json["maxTimeoutMsecs"] = Timeout().count();
  return json;
}

const std::string& WaitForElementAction::Xpath() const {
  return xpath_;
}

const WaitForElementAction::Ms& WaitForElementAction::Timeout() const {
  return timeout_;
}

}