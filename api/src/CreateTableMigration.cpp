#include "CreateTableMigration.h"
#include "DbConverters.h"

namespace api {

CreateTableMigration::CreateTableMigration(
  const std::string& migration_id,
  const SharedPgConnection& connection,
  Params params
) : AbstractDbMigration{migration_id, connection},
    params_{std::move(params)},
    connection_{connection} {
  if (params_.table_name.empty()) {
    throw std::invalid_argument{"Empty table name to create"};
  }

  if (params_.columns.empty()) {
    throw std::invalid_argument{"Empty columns for table to create"};
  }
}

void CreateTableMigration::Execute() const {
  std::string sql = BuildSqlQuery();
  SPDLOG_INFO("Executing query: {:s}", sql);
  pqxx::work work{*connection_};
  work.exec0(sql);
  work.commit();
}

std::string CreateTableMigration::BuildSqlQuery() const {
  std::vector<std::string> column_to_sql;
  column_to_sql.reserve(params_.columns.size());

  std::for_each(begin(params_.columns), end(params_.columns), [&](const ColumnDefinition& column_definition) {
    column_to_sql.push_back(ColumnToPostgreSqlString(column_definition));
  });

  std::vector<std::string> foreign_key_to_sql;
  foreign_key_to_sql.reserve(params_.foreign_keys.size());

  std::for_each(begin(params_.foreign_keys), end(params_.foreign_keys), [&](const ForeignKey& foreign_key) {
    foreign_key_to_sql.push_back(ForeignKeyToPostgreSqlString(foreign_key));
  });

  std::vector<std::string> unique_keys_to_sql;
  unique_keys_to_sql.reserve(params_.columns.size());

  std::for_each(begin(params_.unique_keys), end(params_.unique_keys), [&](const UniqueKey& unique_key) {
    unique_keys_to_sql.push_back(AddUniqueKeyToPostgreSqlStringCreateTable(unique_key));
  });

  std::stringstream sql_stream;

  sql_stream
    << "CREATE TABLE "
    << params_.table_name
    << " ("
    << boost::algorithm::join(column_to_sql, ",\n");

  if (!foreign_key_to_sql.empty()) {
    sql_stream
      << ','
      << boost::algorithm::join(foreign_key_to_sql, ",\n");
  }

  if (!unique_keys_to_sql.empty()) {
    sql_stream
      << ','
      << boost::algorithm::join(unique_keys_to_sql, ",\n");
  }

  sql_stream << ')';
  return sql_stream.str();
}

}