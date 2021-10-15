#include "add_job_handler.h"
#include "errors.h"
#include "http_response_misc.h"
#include "action_factory.h"

namespace {

constexpr const char* kType = "add-job";

}

namespace core {

AddJobHandler::AddJobHandler(std::shared_ptr<PgConnectionPool> pg_pool)
  : pg_pool_{std::move(pg_pool)} {}

IHttpHandler::ExpectedResponse AddJobHandler::Handle(IHttpHandler::RequestType&& request) noexcept {
  const auto request_version = request.version();

  try {
    const auto factory = ActionFactory{};

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

    const auto type = json.at("type").get<std::string>();

    if (type != kType) {
      return common::Unexpected<>{
        MakeErrorCode(Error::kInvalidTypeValue)
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

      actions.push_back(factory.Create(*expected_type, object));
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