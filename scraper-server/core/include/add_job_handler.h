#pragma once

#include "iaction.h"
#include "ihttp_handler.h"
#include "pg_connection_pool.h"

namespace core {

class AddJobHandler : public IHttpHandler {
 public:

  using Actions = std::vector<std::shared_ptr<IAction>>;

  explicit AddJobHandler(std::shared_ptr<PgConnectionPool> pg_pool);
  ExpectedResponse Handle(RequestType&& request) noexcept override;

 private:

  std::shared_ptr<PgConnectionPool> pg_pool_;
};

}// namespace core