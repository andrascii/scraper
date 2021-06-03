#pragma once

#include "abstract_db_migration.h"
#include "pg_connection_factory.h"

namespace api {

class ScraperInitialMigration : public AbstractDbMigration {
 public:
  ScraperInitialMigration();

 private:
  void Execute(const SharedPgConnection& connection) const override;
  void MarkAsApplied(const SharedPgConnection& connection) const override;
  [[nodiscard]] bool IsAlreadyApplied(const SharedPgConnection& connection, const std::string& migration_id) const override;
};

}