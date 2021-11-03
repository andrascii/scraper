#include "wait_for_element_by_xpath_action.h"

namespace core {

WaitForElementByXpathAction::WaitForElementByXpathAction(const nlohmann::json& json) {
  xpath_ = json.at("xPath").get<std::string>();
  timeout_ = Ms{json.at("maxTimeoutMsecs").get<uint64_t>()};
}

WaitForElementByXpathAction::WaitForElementByXpathAction(std::string xpath, WaitForElementByXpathAction::Ms timeout)
    : xpath_{std::move(xpath)},
      timeout_{timeout} {}

nlohmann::json WaitForElementByXpathAction::Serialize() const {
  nlohmann::json json;
  json["xPath"] = Xpath();
  json["maxTimeoutMsecs"] = Timeout().count();
  json["type"] = ActionToString(Type::kWaitForElementByXpath);
  return json;
}

const std::string& WaitForElementByXpathAction::Xpath() const { return xpath_; }

const WaitForElementByXpathAction::Ms& WaitForElementByXpathAction::Timeout() const { return timeout_; }

}// namespace core