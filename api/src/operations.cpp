#include "operations.h"

namespace api {

RandomPauseOperation RandomPauseOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .from_msecs = json.at("fromMsecs").get<uint64_t>(),
    .to_msecs = json.at("toMsecs").get<uint64_t>()
  };
}

WaitForElementOperation WaitForElementOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .xpath = json.at("xPath").get<std::string>(),
    .max_timeout_msecs = json.at("maxTimeoutMsecs").get<uint64_t>()
  };
}

RunScriptOperation RunScriptOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .script = json.at("script").get<std::string>(),
  };
}

GetElementByXPathOperation GetElementByXPathOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .unique_id = json.at("uniqueId").get<bool>(),
    .xpath = json.at("xPath").get<std::string>(),
    .out_field = json.at("outField").get<std::string>(),
  };
}

ExtractFieldFromUrlOperation ExtractFieldFromUrlOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .unique_id = json.at("uniqueId").get<bool>(),
    .reg_exp = json.at("regExp").get<std::string>(),
    .out_field = json.at("outField").get<std::string>(),
  };
}

GetElementByCssSelectorOperation GetElementByCssSelectorOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .unique_id = json.at("uniqueId").get<bool>(),
    .selector = json.at("selector").get<std::string>(),
    .out_field = json.at("outField").get<std::string>(),
  };
}

ClickElementOperation ClickElementOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .xpath = json.at("xPath").get<std::string>(),
  };
}

KeyPressOperation KeyPressOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .key_code = json.at("keyCode").get<uint64_t>(),
  };
}

EmulateMouseMoveOperation EmulateMouseMoveOperation::Create(const nlohmann::json& json) {
  boost::ignore_unused(json);
  return {};
}

TypeTextOnElementByXPathOperation TypeTextOnElementByXPathOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .text = json.at("text").get<std::string>(),
    .xpath = json.at("xPath").get<std::string>(),
  };
}

SetCookiesOperation SetCookiesOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .name = json.at("name").get<std::string>(),
    .value = json.at("value").get<std::string>(),
  };
}

GetImagesByXPathOperation GetImagesByXPathOperation::Create(const nlohmann::json& json) {
  return {
    .optional = json.at("optional").get<bool>(),
    .xpath = json.at("xPath").get<std::string>(),
    .out_field = json.at("outField").get<std::string>(),
  };
}

}