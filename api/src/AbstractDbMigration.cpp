#include "AbstractDbMigration.h"
#include "Finally.h"

namespace api {

AbstractDbMigration::AbstractDbMigration(std::string migration_id, SharedPgConnection connection)
  : migration_id_{std::move(migration_id)},
    connection_{std::move(connection)} {}

void AbstractDbMigration::ExecuteIfNeeded() const {
  if (IsAlreadyApplied(migration_id_)) {
    SPDLOG_INFO("Migration id '{:s}' has already been applied", migration_id_);
    return;
  }

  Execute();
  MarkAsApplied();
}

void AbstractDbMigration::MarkAsApplied() const {
  try {
    pqxx::work work{*connection_};
    work.exec_params("INSERT INTO db_migrations (migration_id) VALUES($1)", migration_id_);
    work.commit();
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    throw;
  }
}

bool AbstractDbMigration::IsAlreadyApplied(const std::string& migration_id) const {
  try {
    pqxx::work work{*connection_};
    const auto result = work.exec_params("SELECT * FROM db_migrations WHERE migration_id=$1", migration_id_);
    const auto has_migration = !result.empty();
    work.commit();
    return has_migration;
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    throw;
  }
}

}