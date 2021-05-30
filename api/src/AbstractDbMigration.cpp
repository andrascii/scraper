#include "AbstractDbMigration.h"

namespace api {

AbstractDbMigration::AbstractDbMigration(std::string migration_id)
  : migration_id_{std::move(migration_id)} {}

void AbstractDbMigration::ExecuteIfNeeded(const SharedPgConnection& connection) const {
  if (IsAlreadyApplied(connection, migration_id_)) {
    SPDLOG_INFO("Migration id '{:s}' has already been applied", migration_id_);
    return;
  }

  Execute(connection);
  MarkAsApplied(connection);
}

void AbstractDbMigration::MarkAsApplied(const SharedPgConnection& connection) const {
  try {
    pqxx::work tx{*connection};
    tx.exec_params("INSERT INTO db_migrations (migration_id) VALUES($1)", migration_id_);
    tx.commit();
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    throw;
  }
}

bool AbstractDbMigration::IsAlreadyApplied(const SharedPgConnection& connection, const std::string& migration_id) const {
  try {
    pqxx::work tx{*connection};
    const auto result = tx.exec_params("SELECT * FROM db_migrations WHERE migration_id=$1", migration_id_);
    const auto has_migration = !result.empty();
    tx.commit();
    return has_migration;
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    throw;
  }
}

}