#include "remove_job_handler.h"
#include "errors.h"
#include "http_response_misc.h"

namespace {

constexpr const char* kType = "remove-job";

}

namespace core {

RemoveJobHandler::RemoveJobHandler(std::shared_ptr<PgConnectionPool> pg_pool)
  : pg_pool_{std::move(pg_pool)} {}

IHttpHandler::ExpectedResponse RemoveJobHandler::Handle(IHttpHandler::RequestType&& request) noexcept {
  const auto request_version = request.version();

  try {
    const auto& body = request.body();
    const auto json = nlohmann::json::parse(body);

    if (!json.is_object()) {
      return common::Unexpected<>{
        MakeErrorCode(Error::kJsonIsNotAnObject)
      };
    }

    const auto type = json.at("type").get<std::string>();

    if (type != kType) {
      return common::Unexpected<>{
        MakeErrorCode(Error::kInvalidTypeValue)
      };
    }

    const auto job_id = json.at("id").get<uint64_t>();
    const auto connection = pg_pool_->Take();
    auto tx = pqxx::work{*connection.Get()};
    tx.exec_params("DELETE FROM actions WHERE job_id = $1", job_id);
    tx.exec_params("DELETE FROM jobs WHERE id = $1", job_id);
    tx.commit();

    return Ok(request_version, "Job successfully removed", ContentType::kTextPlain);
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