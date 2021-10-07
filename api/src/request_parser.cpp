#include "request_parser.h"
#include "errors.h"

namespace {}

namespace api {

RequestParser::ExpectedActions RequestParser::Parse(const IHttpHandler::RequestType& request) const noexcept {
  try {
    const auto& body = request.body();
    const auto json = nlohmann::json::parse(body);

    if (!json.contains("type")) {
      return common::Unexpected<>{MakeErrorCode(Error::kNotFoundTypeFieldInReceivedRequest)};
    }

    [[maybe_unused]] const auto& request_type = json["type"];

    return common::Unexpected<>{MakeErrorCode(Error::kUnknownTypeFieldValueInReceivedRequest)};
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    return {};
  }
}

}