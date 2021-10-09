#pragma once

#include "helpers.h"
#include "ihttp_handler.h"
#include "iaction.h"

namespace api {

class RequestParser final {
 public:
  using Actions = std::vector<std::shared_ptr<IAction>>;
  using ExpectedActions = common::Expected<Actions>;

  static ExpectedActions Parse(const IHttpHandler::RequestType& request) noexcept;
};

}