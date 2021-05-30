#pragma once

#include "IMigrator.h"
#include "IDbMigration.h"
#include "PgConnectionPool.h"

namespace api {

class ScraperMigrator final : public IMigrator {
 public:
  ScraperMigrator(SharedSettings settings, std::shared_ptr<PgConnectionPool> pool);

  void Apply() const override;

 private:
  SharedSettings settings_;
  std::shared_ptr<PgConnectionPool> pool_;
  std::vector<std::unique_ptr<IDbMigration>> migrations_;
};

}