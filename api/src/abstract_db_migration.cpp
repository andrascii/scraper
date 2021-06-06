#include "abstract_db_migration.h"

namespace api {

AbstractDbMigration::AbstractDbMigration(std::string migration_id)
  : migration_id_{std::move(migration_id)} {}

const std::string& AbstractDbMigration::MigrationId() const noexcept {
  return migration_id_;
}

void AbstractDbMigration::ExecuteIfNeeded(const std::shared_ptr<pqxx::connection>& connection) const {
  SPDLOG_INFO("applying {:s} migration", std::quoted(migration_id_));

  if (IsAlreadyApplied(connection, migration_id_)) {
    SPDLOG_INFO("migration {:s} has already been applied", std::quoted(migration_id_));
    return;
  }

  Execute(connection);
  MarkAsApplied(connection);

  SPDLOG_INFO("migration {:s} successfully applied", std::quoted(migration_id_));
}

void AbstractDbMigration::MarkAsApplied(const std::shared_ptr<pqxx::connection>& connection) const {
  try {
    pqxx::work tx{*connection};
    tx.exec_params("INSERT INTO db_migrations (migration_id) VALUES($1)", migration_id_);
    tx.commit();
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    throw;
  }
}

bool AbstractDbMigration::IsAlreadyApplied(const std::shared_ptr<pqxx::connection>& connection, const std::string& migration_id) const {
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