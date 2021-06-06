#pragma once

#include "abstract_db_migration.h"
#include "pg_connection_factory.h"

namespace api {

class ScraperInitialMigration : public AbstractDbMigration {
 public:
  ScraperInitialMigration();

 private:
  void Execute(const std::shared_ptr<pqxx::connection>& connection) const override;
  void MarkAsApplied(const std::shared_ptr<pqxx::connection>& connection) const override;
  [[nodiscard]] bool IsAlreadyApplied(const std::shared_ptr<pqxx::connection>& connection, const std::string& migration_id) const override;
};

}