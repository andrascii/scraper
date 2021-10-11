#pragma once

#include "ihttp_handler.h"
#include "iaction.h"
#include "pg_connection_pool.h"

namespace api {

class AddJobHandler : public IHttpHandler {
 public:
  using Actions = std::vector<std::shared_ptr<IAction>>;
  using ExpectedActions = common::Expected<Actions>;

  explicit AddJobHandler(std::shared_ptr<PgConnectionPool> pg_pool);

  ExpectedResponse Handle(RequestType&& request) noexcept override;

 private:
  std::shared_ptr<PgConnectionPool> pg_pool_;
};

}