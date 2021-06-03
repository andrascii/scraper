#pragma once

#include "imigrator.h"
#include "idb_migration.h"
#include "pg_connection_pool.h"

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