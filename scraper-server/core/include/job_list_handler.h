#pragma once

#include "ihttp_handler.h"
#include "pg_connection_pool.h"

namespace core {

class JobListHandler final : public IHttpHandler {
 public:

  explicit JobListHandler(std::shared_ptr<PgConnectionPool> pg_pool);
  ExpectedResponse Handle(RequestType&& request) noexcept override;

 private:

  std::shared_ptr<PgConnectionPool> pg_pool_;
};

}// namespace core