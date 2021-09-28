#pragma once

#include "helpers.h"

namespace api {

//
// implementations must be thread safe
//
class IHttpHandler {
 public:
  using RequestType = boost::beast::http::request<boost::beast::http::string_body>;
  using ResponseType = boost::beast::http::response<boost::beast::http::string_body>;
  using ExpectedResponse = common::Expected<IHttpHandler::ResponseType>;

  virtual ~IHttpHandler() = default;

  virtual ExpectedResponse Handle(RequestType&& request) noexcept = 0;
};

}