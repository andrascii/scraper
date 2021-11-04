#pragma once

#include "errors.h"
#include "helpers.h"

namespace core {

class IAction {
 public:

  enum Type {
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

  virtual ~IAction() = default;
  [[nodiscard]] virtual nlohmann::json Serialize() const = 0;
};

inline common::Expected<IAction::Type> StringToAction(const std::string& type) noexcept {
  if (boost::iequals(type, "click-element")) {
    return IAction::Type::kClickElement;
  }
  if (boost::iequals(type, "emulate-mouse-move")) {
    return IAction::Type::kEmulateMouseMove;
  }
  if (boost::iequals(type, "extract-field-from-url")) {
    return IAction::Type::kExtractFieldFromUrl;
  }
  if (boost::iequals(type, "get-element-by-css-selector")) {
    return IAction::Type::kGetElementByCssSelector;
  }
  if (boost::iequals(type, "get-element-by-xpath")) {
    return IAction::Type::kGetElementByXpath;
  }
  if (boost::iequals(type, "R")) {
    return IAction::Type::kGetImagesByXpath;
  }
  if (boost::iequals(type, "go-to-url")) {
    return IAction::Type::kGoToUrl;
  }
  if (boost::iequals(type, "key-press")) {
    return IAction::Type::kKeyPress;
  }
  if (boost::iequals(type, "random-pause")) {
    return IAction::Type::kRandomPause;
  }
  if (boost::iequals(type, "run-script")) {
    return IAction::Type::kRunScript;
  }
  if (boost::iequals(type, "set-cookies")) {
    return IAction::Type::kSetCookies;
  }
  if (boost::iequals(type, "type-text-on-element-by-xpath")) {
    return IAction::Type::kTypeTextOnElementByXpath;
  }
  if (boost::iequals(type, "wait-for-element-by-xpath")) {
    return IAction::Type::kWaitForElementByXpath;
  }

  return common::Unexpected<>{MakeErrorCode(Error::kUnknownTypeFieldValueInReceivedRequest)};
}

inline std::string_view ActionToString(IAction::Type type) {
  switch (type) {
    case IAction::Type::kClickElement:
      return "click-element";
    case IAction::Type::kEmulateMouseMove:
      return "emulate-mouse-move";
    case IAction::Type::kExtractFieldFromUrl:
      return "extract-field-from-url";
    case IAction::Type::kGetElementByCssSelector:
      return "get-element-by-css-selector";
    case IAction::Type::kGetElementByXpath:
      return "get-element-by-xpath";
    case IAction::Type::kGetImagesByXpath:
      return "get-images-by-xpath";
    case IAction::Type::kGoToUrl:
      return "go-to-url";
    case IAction::Type::kKeyPress:
      return "key-press";
    case IAction::Type::kRandomPause:
      return "random-pause";
    case IAction::Type::kRunScript:
      return "run-script";
    case IAction::Type::kSetCookies:
      return "set-cookies";
    case IAction::Type::kTypeTextOnElementByXpath:
      return "type-text-on-element-by-xpath";
    case IAction::Type::kWaitForElementByXpath:
      return "wait-for-element-by-xpath";
    default:
      throw std::invalid_argument{"undefined IAction::Type"};
  }
}

}// namespace core