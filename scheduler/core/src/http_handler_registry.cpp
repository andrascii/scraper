#include "http_handler_registry.h"

#include "errors.h"

namespace core {

IHttpHandler::ExpectedResponse
HttpHandlerRegistry::HandleRequest(
  PostHttpHandlerType type,
  IHttpHandler::RequestType&& request) const noexcept {
  std::lock_guard lk{mutex_};
  const auto it = post_handlers_.find(type);

  if (it != end(post_handlers_)) {
    const auto& [key, value] = *it;
    return value->Handle(std::move(request));
  }

  // change it to Bad Request response with HTML page
  SPDLOG_WARN("Requested unregistered POST handler");
  return common::Unexpected<>{MakeErrorCode(Error::kUndefinedPostHttpHandler)};
}

IHttpHandler::ExpectedResponse
HttpHandlerRegistry::HandleRequest(
  GetHttpHandlerType type,
  IHttpHandler::RequestType&& request) const noexcept {
  std::lock_guard lk{mutex_};
  const auto it = get_handlers_.find(type);

  if (it != end(get_handlers_)) {
    const auto& [key, value] = *it;
    return value->Handle(std::move(request));
  }

  // change it to Bad Request response with HTML page
  SPDLOG_WARN("Requested unregistered POST handler");
  return common::Unexpected<>{MakeErrorCode(Error::kUndefinedGetHttpHandler)};
}

void HttpHandlerRegistry::AddPostHandler(PostHttpHandlerType type, const std::shared_ptr<IHttpHandler>& handler) {
  if (!handler) {
    throw std::invalid_argument{"passed pointer to an HTTP handler that is nullptr"};
  }

  std::lock_guard lk{mutex_};
  const auto it = post_handlers_.find(type);

  if (it != end(post_handlers_)) {
    const auto expected_handler_name = FromPostHttpHandlerType(type);

    if (!expected_handler_name) {
      SPDLOG_ERROR(
        "attempt to add already existing HTTP handler but it's name is "
        "undefined");
    } else {
      SPDLOG_WARN("attempt to add already existing HTTP handler: {:s}", *expected_handler_name);
    }

    return;
  }

  post_handlers_[type] = handler;
}

void HttpHandlerRegistry::AddGetHandler(GetHttpHandlerType type, const std::shared_ptr<IHttpHandler>& handler) {
  if (!handler) {
    throw std::invalid_argument{"passed pointer to an HTTP handler that is nullptr"};
  }

  std::lock_guard lk{mutex_};
  const auto it = get_handlers_.find(type);

  if (it != end(get_handlers_)) {
    const auto expected_handler_name = FromGetHttpHandlerType(type);

    if (!expected_handler_name) {
      SPDLOG_ERROR(
        "attempt to add already existing GET HTTP handler but it's name is "
        "undefined");
    } else {
      SPDLOG_WARN("attempt to add already existing GET HTTP handler: {:s}", *expected_handler_name);
    }

    return;
  }
}

void HttpHandlerRegistry::RemovePostHandler(PostHttpHandlerType type) noexcept {
  std::lock_guard lk{mutex_};
  post_handlers_.erase(type);
}

void HttpHandlerRegistry::RemoveGetHandler(GetHttpHandlerType type) noexcept {
  std::lock_guard lk{mutex_};
  get_handlers_.erase(type);
}

}// namespace core
