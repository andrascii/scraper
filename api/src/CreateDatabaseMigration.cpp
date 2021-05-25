#include "CreateDatabaseMigration.h"

#include <utility>

namespace api {

CreateDatabaseMigration::CreateDatabaseMigration(SharedPgConnection connection, std::string database_name)
  : AbstractDbMigration{"database", connection},
    database_name_{std::move(database_name)},
    connection_{std::move(connection)} {
  if (!connection_) {
    throw std::runtime_error{"Passed nullptr to pqxx::connection"};
  }
}

void CreateDatabaseMigration::Execute() const {
  try {
    const auto sql = "CREATE DATABASE " + database_name_ + R"(
        WITH
        OWNER = postgres
        ENCODING = 'UTF8'
        CONNECTION LIMIT = -1;
    )";

    pqxx::work work{*connection_};
    work.exec0(sql);
    work.commit();
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    throw;
  }
}

bool CreateDatabaseMigration::IsAlreadyApplied(const std::string& migration_id) const {
  boost::ignore_unused(migration_id);

  try {
    pqxx::work work{*connection_};
    pqxx::row row = work.exec1("SELECT * FROM pg_database WHERE datname='" + database_name_ + "'");
    work.commit();
    return !row.empty();
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{:s}", ex.what());
    throw;
  }
}

}