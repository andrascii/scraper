#include "request_parser.h"
#include "errors.h"
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

namespace {

using namespace api;

enum class ActionType {
  kClickElement,
  kEmulateMouseMove,
  kExtractFieldFromUrl,
  kGetElementByCssSelector,
  kGetElementByXpath,
  kGetImagesByXpath,
  kGoToUrl,
  kKeyPress,
  kRandomPause,
  kRunScript,
  kSetCookies,
  kTypeTextOnElementByXpath,
  kWaitForElementByXpath,
};

common::Expected<ActionType> StringToActionType(const std::string& type) noexcept {
  if (boost::iequals(type, "click-element")) {
    return ActionType::kClickElement;
  }
  if (boost::iequals(type, "emulate-mouse-move")) {
    return ActionType::kEmulateMouseMove;
  }
  if (boost::iequals(type, "extract-field-from-url")) {
    return ActionType::kExtractFieldFromUrl;
  }
  if (boost::iequals(type, "get-element-by-css-selector")) {
    return ActionType::kGetElementByCssSelector;
  }
  if (boost::iequals(type, "get-element-by-xpath")) {
    return ActionType::kGetElementByXpath;
  }
  if (boost::iequals(type, "get-images-by-xpath")) {
    return ActionType::kGetImagesByXpath;
  }
  if (boost::iequals(type, "go-to-url")) {
    return ActionType::kGoToUrl;
  }
  if (boost::iequals(type, "key-press")) {
    return ActionType::kKeyPress;
  }
  if (boost::iequals(type, "random-pause")) {
    return ActionType::kRandomPause;
  }
  if (boost::iequals(type, "run-script")) {
    return ActionType::kRunScript;
  }
  if (boost::iequals(type, "set-cookies")) {
    return ActionType::kSetCookies;
  }
  if (boost::iequals(type, "type-text-on-element-by-xpath")) {
    return ActionType::kTypeTextOnElementByXpath;
  }
  if (boost::iequals(type, "wait-for-element-by-xpath")) {
    return ActionType::kWaitForElementByXpath;
  }

  return common::Unexpected<>{
    MakeErrorCode(Error::kUnknownTypeFieldValueInReceivedRequest)
  };
}

std::shared_ptr<IAction> CreateAction(ActionType type, const nlohmann::json& json) {
  switch (type) {
    case ActionType::kClickElement: {
      return std::make_shared<ClickElementAction>(json);
    }
    case ActionType::kEmulateMouseMove: {
      return std::make_shared<EmulateMouseMoveAction>(json);
    }
    case ActionType::kExtractFieldFromUrl: {
      return std::make_shared<ExtractFieldFromUrlAction>(json);
    }
    case ActionType::kGetElementByCssSelector: {
      return std::make_shared<GetElementByCssSelectorAction>(json);
    }
    case ActionType::kGetElementByXpath: {
      return std::make_shared<GetElementByXpathAction>(json);
    }
    case ActionType::kGetImagesByXpath: {
      return std::make_shared<GetImagesByXpathAction>(json);
    }
    case ActionType::kGoToUrl: {
      return std::make_shared<GoToUrlAction>(json);
    }
    case ActionType::kKeyPress: {
      return std::make_shared<KeyPressAction>(json);
    }
    case ActionType::kRandomPause: {
      return std::make_shared<RandomPauseAction>(json);
    }
    case ActionType::kRunScript: {
      return std::make_shared<RunScriptAction>(json);
    }
    case ActionType::kSetCookies: {
      return std::make_shared<SetCookiesAction>(json);
    }
    case ActionType::kTypeTextOnElementByXpath: {
      return std::make_shared<TypeTextOnElementByXpathAction>(json);
    }
    case ActionType::kWaitForElementByXpath: {
      return std::make_shared<WaitForElementByXpathAction>(json);
    }
    default: {
      throw std::invalid_argument{"Received unknown ActionType"};
    }
  }
}

}

namespace api {

RequestParser::ExpectedActions RequestParser::Parse(const IHttpHandler::RequestType& request) noexcept {
  try {
    const auto& body = request.body();
    const auto json = nlohmann::json::parse(body);

    auto actions = Actions{};

    if (!json.is_array()) {
      return common::Unexpected<>{
        MakeErrorCode(Error::kJsonIsNotAnArray)
      };
    }

    for (const auto& object : json) {
      if (!object.is_object()) {
        return common::Unexpected<>{
          MakeErrorCode(Error::kJsonIsNotAnObject)
        };
      }

      if (!object.contains("type")) {
        return common::Unexpected<>{
          MakeErrorCode(Error::kNotFoundTypeFieldInReceivedRequest)
        };
      }

      const auto expected_action_type = StringToActionType(object.at("type").get<std::string>());

      if (expected_action_type) {
        actions.push_back(CreateAction(*expected_action_type, object));
      } else {
        return common::Unexpected<>{expected_action_type.error()};
      }
    }

    return actions;
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    return {};
  }
}

}
