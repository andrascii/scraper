#pragma once

#include "helpers.h"
#include "errors.h"
#include "ihttp_handler.h"

namespace api {

enum class PostHttpHandlerType {
  kAddCrawlJob,
  kRemoveCrawlJob,
  kEnableCrawlJob,
  kDisableCrawlJob,
  kUpdateCrawlJob,
  kCrawlJobList,
  kAddReportJob,
  kRemoveReportJob,
  kReportJobList,
  kGetReportResults,
};

inline tl::expected<PostHttpHandlerType, std::error_code> ToPostHttpHandlerType(const std::string& name) noexcept {
  if (boost::iequals(name, "add-crawl-job")) return PostHttpHandlerType::kAddCrawlJob;
  if (boost::iequals(name, "remove-crawl-job")) return PostHttpHandlerType::kRemoveCrawlJob;
  if (boost::iequals(name, "enable-crawl-job")) return PostHttpHandlerType::kEnableCrawlJob;
  if (boost::iequals(name, "disable-crawl-job")) return PostHttpHandlerType::kDisableCrawlJob;
  if (boost::iequals(name, "update-crawl-job")) return PostHttpHandlerType::kUpdateCrawlJob;
  if (boost::iequals(name, "crawl-job-list")) return PostHttpHandlerType::kCrawlJobList;
  if (boost::iequals(name, "add-report-job")) return PostHttpHandlerType::kAddReportJob;
  if (boost::iequals(name, "remove-report-job")) return PostHttpHandlerType::kRemoveReportJob;
  if (boost::iequals(name, "report-job-list")) return PostHttpHandlerType::kReportJobList;
  if (boost::iequals(name, "get-report-results")) return PostHttpHandlerType::kGetReportResults;
  return tl::unexpected<std::error_code>{MakeErrorCode(Error::kUndefinedPostHttpHandler)};
}

inline tl::expected<std::string, std::error_code> FromPostHttpHandlerType(PostHttpHandlerType type) {
  switch (type) {
    case PostHttpHandlerType::kAddCrawlJob: return "add-crawl-job";
    case PostHttpHandlerType::kRemoveCrawlJob: return "remove-crawl-job";
    case PostHttpHandlerType::kEnableCrawlJob: return "enable-crawl-job";
    case PostHttpHandlerType::kDisableCrawlJob: return "disable-crawl-job";
    case PostHttpHandlerType::kUpdateCrawlJob: return "update-crawl-job";
    case PostHttpHandlerType::kCrawlJobList: return "crawl-job-list";
    case PostHttpHandlerType::kAddReportJob: return "add-report-job";
    case PostHttpHandlerType::kRemoveReportJob: return "remove-report-job";
    case PostHttpHandlerType::kReportJobList: return "report-job-list";
    case PostHttpHandlerType::kGetReportResults: return "get-report-results";
    default: return tl::unexpected<std::error_code>{MakeErrorCode(Error::kUndefinedPostHttpHandler)};
  }
}

enum class GetHttpHandlerType {
  kRootPage,
  kWebAdminJs,
  kReportsPage,
  kBundleJs,
};

inline tl::expected<PostHttpHandlerType, std::error_code> ToGetHttpHandlerType(const std::string& name) noexcept {
  return tl::unexpected<std::error_code>{MakeErrorCode(Error::kUndefinedPostHttpHandler)};
}

inline tl::expected<std::string, std::error_code> FromGetHttpHandlerType(GetHttpHandlerType type) {
  return tl::unexpected<std::error_code>{MakeErrorCode(Error::kUndefinedPostHttpHandler)};
}

//
// implementations must be thread safe
//
class IHttpHandlerRegistry {
 public:
  virtual ~IHttpHandlerRegistry() = default;

  virtual IHttpHandler::ExpectedResponse HandleRequest(PostHttpHandlerType type, IHttpHandler::RequestType&& request) const noexcept = 0;
  virtual IHttpHandler::ExpectedResponse HandleRequest(GetHttpHandlerType type, IHttpHandler::RequestType&& request) const noexcept = 0;

  virtual void AddPostHandler(PostHttpHandlerType type, const std::shared_ptr<IHttpHandler>& handler) = 0;
  virtual void AddGetHandler(GetHttpHandlerType type, const std::shared_ptr<IHttpHandler>& handler) = 0;

  virtual void RemovePostHandler(PostHttpHandlerType type) noexcept = 0;
  virtual void RemoveGetHandler(GetHttpHandlerType type) noexcept = 0;
};

}