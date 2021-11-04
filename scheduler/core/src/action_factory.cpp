#include "action_factory.h"

#include "click_element_action.h"
#include "emulate_mouse_move_action.h"
#include "extract_field_from_url_action.h"
#include "get_element_by_css_selector_action.h"
#include "get_element_by_xpath_action.h"
#include "get_images_by_xpath_action.h"
#include "go_to_url_action.h"
#include "key_press_action.h"
#include "random_pause_action.h"
#include "run_script_action.h"
#include "set_cookies_action.h"
#include "type_text_on_element_by_xpath_action.h"
#include "wait_for_element_by_xpath_action.h"

namespace core {

std::shared_ptr<IAction> ActionFactory::Create(IAction::Type type, const nlohmann::json& json) const {
  try {
    switch (type) {
      case IAction::Type::kClickElement: {
        return std::make_shared<ClickElementAction>(json);
      }
      case IAction::Type::kEmulateMouseMove: {
        return std::make_shared<EmulateMouseMoveAction>(json);
      }
      case IAction::Type::kExtractFieldFromUrl: {
        return std::make_shared<ExtractFieldFromUrlAction>(json);
      }
      case IAction::Type::kGetElementByCssSelector: {
        return std::make_shared<GetElementByCssSelectorAction>(json);
      }
      case IAction::Type::kGetElementByXpath: {
        return std::make_shared<GetElementByXpathAction>(json);
      }
      case IAction::Type::kGetImagesByXpath: {
        return std::make_shared<GetImagesByXpathAction>(json);
      }
      case IAction::Type::kGoToUrl: {
        return std::make_shared<GoToUrlAction>(json);
      }
      case IAction::Type::kKeyPress: {
        return std::make_shared<KeyPressAction>(json);
      }
      case IAction::Type::kRandomPause: {
        return std::make_shared<RandomPauseAction>(json);
      }
      case IAction::Type::kRunScript: {
        return std::make_shared<RunScriptAction>(json);
      }
      case IAction::Type::kSetCookies: {
        return std::make_shared<SetCookiesAction>(json);
      }
      case IAction::Type::kTypeTextOnElementByXpath: {
        return std::make_shared<TypeTextOnElementByXpathAction>(json);
      }
      case IAction::Type::kWaitForElementByXpath: {
        return std::make_shared<WaitForElementByXpathAction>(json);
      }
      default: {
        throw std::invalid_argument{"Received unknown IAction::Type"};
      }
    }
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("error deserializing action '{}'", ActionToString(type));
    throw;
  }
}

}// namespace core
