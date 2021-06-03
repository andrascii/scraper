#include "scraper_initial_migration.h"

namespace api {

ScraperInitialMigration::ScraperInitialMigration()
  : AbstractDbMigration{"fake_migration_id"} {}

void ScraperInitialMigration::Execute(const SharedPgConnection& connection) const {
  try {
    const auto create_db_migrations_table_sql = R"(
      CREATE TABLE db_migrations (
        migration_id VARCHAR (150) PRIMARY KEY
      )
    )";

    pqxx::work tx{*connection};
    tx.exec0(create_db_migrations_table_sql);
    tx.commit();
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    throw;
  }
}

void ScraperInitialMigration::MarkAsApplied(const SharedPgConnection& connection) const {
  boost::ignore_unused(connection);
}

bool ScraperInitialMigration::IsAlreadyApplied(const SharedPgConnection& connection, const std::string& migration_id) const {
  boost::ignore_unused(migration_id);

  try {
    pqxx::work tx{*connection};
    pqxx::result result = tx.exec("SELECT EXISTS (SELECT FROM pg_tables WHERE tablename='db_migrations')");
    tx.commit();

    bool exists = false;

    if (!std::empty(result)) {
      exists = result[0][0].as<bool>();
    }

    return exists;
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    throw;
  }
}

}