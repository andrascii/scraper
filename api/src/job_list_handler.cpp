#include "job_list_handler.h"
#include "http_response_misc.h"
#include "errors.h"

namespace {

constexpr const char* kType = "job-list";

struct Job {
  uint64_t id{};
  bool enabled{};
  uint64_t update_frequency{};
  std::vector<std::string> actions;
};

}

namespace api {

JobListHandler::JobListHandler(std::shared_ptr<PgConnectionPool> pg_pool)
  : pg_pool_{std::move(pg_pool)} {}

IHttpHandler::ExpectedResponse JobListHandler::Handle(IHttpHandler::RequestType&& request) noexcept {
  const auto request_version = request.version();

  try {
    const auto& body = request.body();
    const auto json = nlohmann::json::parse(body);

    const auto type = json.at("type").get<std::string>();

    if (type != kType) {
      return common::Unexpected<>{
        MakeErrorCode(Error::kInvalidTypeValue)
      };
    }

    auto jobs = std::vector<Job>{};
    const auto connection = pg_pool_->Take();
    auto tx = pqxx::work{*connection.Get()};
    const auto result = tx.exec("SELECT id, enabled, update_frequency FROM jobs");

    for (const auto& row : result) {
      auto job = Job{};
      job.id = row[0].as<uint64_t>();
      job.enabled = row[1].as<bool>();
      job.update_frequency = row[2].as<uint64_t>();

      const auto job_actions = tx.exec_params("SELECT action_json FROM actions WHERE job_id = $1", job.id);
      for (const auto& row : job_actions) {
        job.actions.push_back(row[0].as<std::string>());
      }

      jobs.push_back(job);
    }

    auto jobs_json = nlohmann::json::array();

    for (const auto& job : jobs) {
      nlohmann::json job_json;
      job_json["id"] = job.id;
      job_json["enabled"] = job.enabled;
      job_json["updateFrequency"] = job.update_frequency;
      job_json["actions"] = nlohmann::json::array();

      for (const auto& action : job.actions) {
        job_json["actions"].push_back(action);
      }

      jobs_json.push_back(job_json);
    }

    tx.commit();

    return Ok(request_version, jobs_json.dump(), ContentType::kTextPlain);
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