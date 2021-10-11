#pragma once

#include "iaction_visitor.h"
#include "helpers.h"
#include "errors.h"

namespace api {

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

inline common::Expected<ActionType> StringToAction(const std::string& type) noexcept {
  if (boost::iequals(type, "click-element")) { return ActionType::kClickElement; }
  if (boost::iequals(type, "emulate-mouse-move")) { return ActionType::kEmulateMouseMove; }
  if (boost::iequals(type, "extract-field-from-url")) { return ActionType::kExtractFieldFromUrl; }
  if (boost::iequals(type, "get-element-by-css-selector")) { return ActionType::kGetElementByCssSelector; }
  if (boost::iequals(type, "get-element-by-xpath")) { return ActionType::kGetElementByXpath; }
  if (boost::iequals(type, "get-images-by-xpath")) { return ActionType::kGetImagesByXpath; }
  if (boost::iequals(type, "go-to-url")) { return ActionType::kGoToUrl; }
  if (boost::iequals(type, "key-press")) { return ActionType::kKeyPress; }
  if (boost::iequals(type, "random-pause")) { return ActionType::kRandomPause; }
  if (boost::iequals(type, "run-script")) { return ActionType::kRunScript; }
  if (boost::iequals(type, "set-cookies")) { return ActionType::kSetCookies; }
  if (boost::iequals(type, "type-text-on-element-by-xpath")) { return ActionType::kTypeTextOnElementByXpath; }
  if (boost::iequals(type, "wait-for-element-by-xpath")) { return ActionType::kWaitForElementByXpath; }

  return common::Unexpected<>{
    MakeErrorCode(Error::kUnknownTypeFieldValueInReceivedRequest)
  };
}

inline std::string_view ActionToString(ActionType type) {
  switch(type) {
    case ActionType::kClickElement: return "click-element";
    case ActionType::kEmulateMouseMove: return "emulate-mouse-move";
    case ActionType::kExtractFieldFromUrl: return "extract-field-from-url";
    case ActionType::kGetElementByCssSelector: return "get-element-by-css-selector";
    case ActionType::kGetElementByXpath: return "get-element-by-xpath";
    case ActionType::kGetImagesByXpath: return "get-images-by-xpath";
    case ActionType::kGoToUrl: return "go-to-url";
    case ActionType::kKeyPress: return "key-press";
    case ActionType::kRandomPause: return "random-pause";
    case ActionType::kRunScript: return "run-script";
    case ActionType::kSetCookies: return "set-cookies";
    case ActionType::kTypeTextOnElementByXpath: return "type-text-on-element-by-xpath";
    case ActionType::kWaitForElementByXpath: return "wait-for-element-by-xpath";
    default: throw std::invalid_argument{"undefined ActionType"};
  }
}

class IAction {
 public:
  virtual ~IAction() = default;
  virtual void Accept(IActionVisitor* visitor) const = 0;
  [[nodiscard]] virtual nlohmann::json Serialize() const = 0;
};

}