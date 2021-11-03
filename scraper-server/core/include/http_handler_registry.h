#pragma once

#include "ihttp_handler_registry.h"

namespace core {

//
// thread-safe
//
class HttpHandlerRegistry final : public IHttpHandlerRegistry {
 public:

  IHttpHandler::ExpectedResponse HandleRequest(PostHttpHandlerType type, IHttpHandler::RequestType&& request) const noexcept override;

  IHttpHandler::ExpectedResponse HandleRequest(GetHttpHandlerType type, IHttpHandler::RequestType&& request) const noexcept override;

  void AddPostHandler(PostHttpHandlerType type, const std::shared_ptr<IHttpHandler>& handler) override;
  void AddGetHandler(GetHttpHandlerType type, const std::shared_ptr<IHttpHandler>& handler) override;

  void RemovePostHandler(PostHttpHandlerType type) noexcept override;
  void RemoveGetHandler(GetHttpHandlerType type) noexcept override;

 private:

  using HandlerPtr = std::shared_ptr<IHttpHandler>;

  mutable std::mutex mutex_;
  std::unordered_map<GetHttpHandlerType, HandlerPtr> get_handlers_;
  std::unordered_map<PostHttpHandlerType, HandlerPtr> post_handlers_;
};

}// namespace core