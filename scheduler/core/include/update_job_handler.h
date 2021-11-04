#pragma once

#include "iaction.h"
#include "ihttp_handler.h"
#include "pg_connection_pool.h"

namespace core {

class UpdateJobHandler final : public IHttpHandler {
 public:

  using Actions = std::vector<std::shared_ptr<IAction>>;

  explicit UpdateJobHandler(std::shared_ptr<PgConnectionPool> pg_pool);
  ExpectedResponse Handle(RequestType&& request) noexcept override;

 private:

  std::shared_ptr<PgConnectionPool> pg_pool_;
};

}// namespace core