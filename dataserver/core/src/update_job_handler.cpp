#include "update_job_handler.h"
#include "errors.h"
#include "action_factory.h"
#include "http_response_misc.h"

namespace {

constexpr const char* kType = "update-job";

}

namespace core {

UpdateJobHandler::UpdateJobHandler(std::shared_ptr<PgConnectionPool> pg_pool)
  : pg_pool_{std::move(pg_pool)} {}

IHttpHandler::ExpectedResponse UpdateJobHandler::Handle(RequestType&& request) noexcept {
  const auto request_version = request.version();

  try {
    auto actions = Actions{};
    const auto factory = ActionFactory{};

    const auto& body = request.body();
    const auto json = nlohmann::json::parse(body);
    const auto job_id = json.at("id").get<uint64_t>();
    const auto update_frequency = json.at("updateFrequency").get<uint64_t>();
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
    tx.exec_params("DELETE FROM actions WHERE job_id = $1", job_id);
    tx.exec_params("UPDATE jobs SET update_frequency = $1 WHERE id = $2", update_frequency, job_id);

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