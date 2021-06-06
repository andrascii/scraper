#pragma once

#include "idb_migration.h"
#include "pg_connection_factory.h"

namespace api {

class AbstractDbMigration : public IDbMigration {
public:
  AbstractDbMigration(std::string migration_id);

  const std::string& MigrationId() const noexcept override;
  void ExecuteIfNeeded(const std::shared_ptr<pqxx::connection>& connection) const override;

private:
  virtual void MarkAsApplied(const std::shared_ptr<pqxx::connection>& connection) const;
  bool IsAlreadyApplied(const std::shared_ptr<pqxx::connection>& connection, const std::string& migration_id) const override;

private:
  std::string migration_id_;
};

}
