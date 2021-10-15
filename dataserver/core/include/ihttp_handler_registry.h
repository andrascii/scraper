#pragma once

#include "helpers.h"
#include "errors.h"
#include "ihttp_handler.h"

namespace core {

enum class PostHttpHandlerType {
  kAddJob,
  kRemoveJob,
  kEnableJob,
  kDisableJob,
  kUpdateJob,
  kJobList,
  kAddReportJob,
  kRemoveReportJob,
  kReportJobList,
  kGetReportResults,
};

inline common::Expected<PostHttpHandlerType> ToPostHttpHandlerType(const std::string& name) noexcept {
  if (boost::iequals(name, "add-job")) { return PostHttpHandlerType::kAddJob; }
  if (boost::iequals(name, "remove-job")) { return PostHttpHandlerType::kRemoveJob; }
  if (boost::iequals(name, "enable-job")) { return PostHttpHandlerType::kEnableJob; }
  if (boost::iequals(name, "disable-job")) { return PostHttpHandlerType::kDisableJob; }
  if (boost::iequals(name, "update-job")) { return PostHttpHandlerType::kUpdateJob; }
  if (boost::iequals(name, "job-list")) { return PostHttpHandlerType::kJobList; }
  if (boost::iequals(name, "add-report-job")) { return PostHttpHandlerType::kAddReportJob; }
  if (boost::iequals(name, "remove-report-job")) { return PostHttpHandlerType::kRemoveReportJob; }
  if (boost::iequals(name, "report-job-list")) { return PostHttpHandlerType::kReportJobList; }
  if (boost::iequals(name, "get-report-results")) { return PostHttpHandlerType::kGetReportResults; }
  return common::Unexpected<>{MakeErrorCode(Error::kUndefinedPostHttpHandler)};
}

inline common::Expected<std::string> FromPostHttpHandlerType(PostHttpHandlerType type) {
  switch (type) {
    case PostHttpHandlerType::kAddJob: return "add-job";
    case PostHttpHandlerType::kRemoveJob: return "remove-job";
    case PostHttpHandlerType::kEnableJob: return "enable-job";
    case PostHttpHandlerType::kDisableJob: return "disable-job";
    case PostHttpHandlerType::kUpdateJob: return "update-job";
    case PostHttpHandlerType::kJobList: return "job-list";
    case PostHttpHandlerType::kAddReportJob: return "add-report-job";
    case PostHttpHandlerType::kRemoveReportJob: return "remove-report-job";
    case PostHttpHandlerType::kReportJobList: return "report-job-list";
    case PostHttpHandlerType::kGetReportResults: return "get-report-results";
    default: return common::Unexpected<>{MakeErrorCode(Error::kUndefinedPostHttpHandler)};
  }
}

enum class GetHttpHandlerType {
  kRootPage,
  kWebAdminJs,
  kReportsPage,
  kBundleJs,
};

inline common::Expected<PostHttpHandlerType> ToGetHttpHandlerType(const std::string& name) noexcept {
  return common::Unexpected<>{MakeErrorCode(Error::kUndefinedPostHttpHandler)};
}

inline common::Expected<std::string> FromGetHttpHandlerType(GetHttpHandlerType type) {
  return common::Unexpected<>{MakeErrorCode(Error::kUndefinedGetHttpHandler)};
}

//
// implementations must be thread safe
//
class IHttpHandlerRegistry {
 public:
  virtual ~IHttpHandlerRegistry() = default;

  virtual IHttpHandler::ExpectedResponse HandleRequest(
    PostHttpHandlerType type,
    IHttpHandler::RequestType&& request
  ) const noexcept = 0;

  virtual IHttpHandler::ExpectedResponse HandleRequest(
    GetHttpHandlerType type,
    IHttpHandler::RequestType&& request
  ) const noexcept = 0;

  virtual void AddPostHandler(PostHttpHandlerType type, const std::shared_ptr<IHttpHandler>& handler) = 0;
  virtual void AddGetHandler(GetHttpHandlerType type, const std::shared_ptr<IHttpHandler>& handler) = 0;

  virtual void RemovePostHandler(PostHttpHandlerType type) noexcept = 0;
  virtual void RemoveGetHandler(GetHttpHandlerType type) noexcept = 0;
};

}