#pragma once

#include "ihttp_handler.h"

namespace api {

struct RandomPauseOperation final {
  bool optional{};
  uint64_t from_msecs{};
  uint64_t to_msecs{};

  static RandomPauseOperation Create(const nlohmann::json& json);
};

struct WaitForElementOperation final {
  bool optional{};
  std::string xpath;
  uint64_t max_timeout_msecs{};

  static WaitForElementOperation Create(const nlohmann::json& json);
};

struct RunScriptOperation final {
  bool optional{};
  std::string script;

  static RunScriptOperation Create(const nlohmann::json& json);
};

struct GetElementByXPathOperation final {
  bool optional{};
  bool unique_id{};
  std::string xpath;
  std::string out_field;

  static GetElementByXPathOperation Create(const nlohmann::json& json);
};

struct ExtractFieldFromUrlOperation final {
  bool optional{};
  bool unique_id{};
  std::string reg_exp;
  std::string out_field;

  static ExtractFieldFromUrlOperation Create(const nlohmann::json& json);
};

struct GetElementByCssSelectorOperation final {
  bool optional{};
  bool unique_id{};
  std::string selector;
  std::string out_field;

  static GetElementByCssSelectorOperation Create(const nlohmann::json& json);
};

struct ClickElementOperation final {
  bool optional{};
  std::string xpath;

  static ClickElementOperation Create(const nlohmann::json& json);
};

struct KeyPressOperation final {
  bool optional{};
  uint64_t key_code{};

  static KeyPressOperation Create(const nlohmann::json& json);
};

struct EmulateMouseMoveOperation final {
  bool optional{};

  static EmulateMouseMoveOperation Create(const nlohmann::json& json);
};

struct TypeTextOnElementByXPathOperation final {
  bool optional{};
  std::string text;
  std::string xpath;

  static TypeTextOnElementByXPathOperation Create(const nlohmann::json& json);
};

struct SetCookiesOperation final {
  bool optional{};
  std::string name;
  std::string value;

  static SetCookiesOperation Create(const nlohmann::json& json);
};

struct GetImagesByXPathOperation final {
  bool optional{};
  std::string xpath;
  std::string out_field;

  static GetImagesByXPathOperation Create(const nlohmann::json& json);
};

using Operation = std::variant<
  std::monostate,
  RandomPauseOperation,
  WaitForElementOperation,
  RunScriptOperation,
  GetElementByXPathOperation,
  ExtractFieldFromUrlOperation,
  GetElementByCssSelectorOperation,
  ClickElementOperation,
  KeyPressOperation,
  EmulateMouseMoveOperation,
  TypeTextOnElementByXPathOperation,
  SetCookiesOperation,
  GetImagesByXPathOperation
>;

}