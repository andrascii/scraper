#pragma once

#include "ihttp_handler.h"
#include "pg_connection_pool.h"

namespace api {

class AddJobHandler final : public IHttpHandler {
 public:
  AddJobHandler(const std::shared_ptr<PgConnectionPool>& pool);

  ExpectedResponse Handle(RequestType&& request) noexcept override;

 private:
  std::shared_ptr<PgConnectionPool> pool_;
};

}