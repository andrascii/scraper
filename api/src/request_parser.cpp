#include "request_parser.h"
#include "errors.h"
#include "add_job_request.h"

namespace {

constexpr std::string_view kAddJobRequest{"add-job-request"};

}

namespace api {

RequestParser::ExpectedParsedRequest RequestParser::Parse(const IHttpHandler::RequestType& request) const noexcept {
  try {
    const auto& body = request.body();
    const auto json = nlohmann::json::parse(body);

    if (!json.contains("type")) {
      return tl::unexpected<std::error_code>{MakeErrorCode(Error::kNotFoundTypeFieldInReceivedRequest)};
    }

    const auto& request_type = json["type"];

    if (request_type == kAddJobRequest) {
      return ParseAddJobRequest(json);
    }

    return tl::unexpected<std::error_code>{MakeErrorCode(Error::kUnknownTypeFieldValueInReceivedRequest)};
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    return {};
  }
}

RequestParser::ExpectedParsedRequest RequestParser::ParseAddJobRequest(const nlohmann::json& json) const noexcept {
  try {
    const auto contains_all_fields =
      json.contains("enabled") &&
      json.contains("url") &&
      json.contains("entryPoints") &&
      json.contains("siteMapUrl") &&
      json.contains("getLinksFromSiteMapOnly") &&
      json.contains("updateFrequency") &&
      json.contains("ignoreMasks") &&
      json.contains("matchMasks") &&
      json.contains("parseAnchors") &&
      json.contains("parseQueries") &&
      json.contains("operations") &&
      json.contains("paginationOperations");

    if (!contains_all_fields) {
      return tl::unexpected<std::error_code>{MakeErrorCode(Error::kNotFoundAllRequiredFieldsInAddJobRequest)};
    }

    Job job{
      .enabled = json.at("enabled").get<bool>(),
      .parse_anchors = json.at("parseAnchors").get<bool>(),
      .parse_queries = json.at("parseQueries").get<bool>(),
      .get_links_from_sitemap_only = json.at("getLinksFromSiteMapOnly").get<bool>(),
      .url = json.at("url").get<std::string>(),
      .sitemap_url = json.at("siteMapUrl").get<std::string>(),
      .update_frequency = json.at("updateFrequency").get<uint64_t>(),
    };

    return AddJobRequest{ Job{
      .enabled = json.at("enabled").get<bool>(),
      .parse_anchors = json.at("parseAnchors").get<bool>(),
      .parse_queries = json.at("parseQueries").get<bool>(),
      .get_links_from_sitemap_only = json.at("getLinksFromSiteMapOnly").get<bool>(),
      .url = json.at("url").get<std::string>(),
      .entry_points = {},
      .ignore_masks = {},
      .match_masks = {},
      .sitemap_url = "",
      .update_frequency = 0,
    }};
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    return {};
  }
}

}