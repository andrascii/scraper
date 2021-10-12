#include "add_job_handler.h"
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
#include "http_response_misc.h"

namespace {

using namespace api;

std::shared_ptr<IAction> CreateAction(ActionType type, const nlohmann::json& json) {
  try {
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
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("error deserializing action '{}'", ActionToString(type));
    throw;
  }
}

}

namespace api {

AddJobHandler::AddJobHandler(std::shared_ptr<PgConnectionPool> pg_pool)
  : pg_pool_{std::move(pg_pool)} {}

IHttpHandler::ExpectedResponse AddJobHandler::Handle(IHttpHandler::RequestType&& request) noexcept {
  const auto request_version = request.version();

  try {
    const auto& body = request.body();
    const auto json = nlohmann::json::parse(body);
    const auto update_frequency = json.at("updateFrequency").get<uint64_t>();

    auto actions = Actions{};

    const auto actions_json_array = json.at("actions");

    if (!actions_json_array.is_array()) {
      return common::Unexpected<>{
        MakeErrorCode(Error::kJsonIsNotAnArray)
      };
    }

    const auto connection = pg_pool_->Take();
    auto tx = pqxx::work{*connection.Get()};

    const auto result = tx.exec_params(
      "INSERT INTO jobs (update_frequency, enabled) VALUES ($1, $2) RETURNING id",
      update_frequency,
      true
    );

    const auto job_id = result[0][0].as<uint64_t>();

    for (const auto& object : actions_json_array) {
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

      const auto action_type_str = object.at("type").get<std::string>();
      const auto expected_type = StringToAction(action_type_str);

      if (!expected_type) {
        SPDLOG_ERROR("Undefined action type: {}", action_type_str);

        return BadRequest(
          request_version,
          expected_type.error().message(),
          ContentType::kTextPlain
        );
      }

      actions.push_back(CreateAction(*expected_type, object));
      tx.exec_params("INSERT INTO actions (job_id, action_json) VALUES ($1, $2)", job_id, object.dump());
    }

    std::string response;
    for (const auto& action : actions) {
      response += action->Serialize().dump() + '\n';
    }

    tx.commit();

    return Ok(request_version, response, ContentType::kTextPlain);
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    return BadRequest(
      request_version,
      ex.what(),
      ContentType::kTextPlain
    );
  }
}

}