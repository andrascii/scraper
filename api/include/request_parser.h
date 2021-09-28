#pragma once

#include "helpers.h"
#include "ihttp_handler.h"
#include "add_job_request.h"

namespace api {

class RequestParser final {
 public:
  using ParsedRequest = std::variant<AddJobRequest>;
  using ExpectedParsedRequest = tl::expected<ParsedRequest, std::error_code>;

  ExpectedParsedRequest Parse(const IHttpHandler::RequestType& request) const noexcept;

 private:
  ExpectedParsedRequest ParseAddJobRequest(const nlohmann::json& json) const noexcept;
};

}