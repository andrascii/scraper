#include "AlterTableMigration.h"
#include "DbConverters.h"

namespace api {

AlterTableMigration::AlterTableMigration(
  const std::string& migration_id,
  const SharedPgConnection& connection,
  Params params
) : AbstractDbMigration{migration_id, connection},
    params_{std::move(params)},
    connection_{connection} {
  if (params_.table_name.empty()) {
    throw std::invalid_argument{"Empty table name to create"};
  }
}

void AlterTableMigration::Execute() const {
  std::string sql = BuildSqlQuery();
  SPDLOG_INFO("Executing query: {:s}", sql);
  pqxx::work work{*connection_};
  work.exec0(sql);
  work.commit();
}

std::string AlterTableMigration::BuildSqlQuery() const {
  std::vector<std::string> drop_column_sql_operations;
  drop_column_sql_operations.reserve(params_.drop_columns.size());
  std::for_each(begin(params_.drop_columns), end(params_.drop_columns), [&](const std::string& column) {
    drop_column_sql_operations.push_back(DropColumnString(column));
  });

  std::vector<std::string> add_column_sql_operations;
  add_column_sql_operations.reserve(params_.add_columns.size());
  std::for_each(begin(params_.add_columns), end(params_.add_columns), [&](const ColumnDefinition& column) {
    add_column_sql_operations.push_back(ColumnToPostgreSqlString(column, "ADD COLUMN "));
  });

  std::vector<std::string> add_foreign_keys_sql_operations;
  add_foreign_keys_sql_operations.reserve(params_.add_foreign_keys.size());
  std::for_each(begin(params_.add_foreign_keys), end(params_.add_foreign_keys), [&](const ForeignKey& key) {
    add_foreign_keys_sql_operations.push_back(ForeignKeyToPostgreSqlString(key, "ADD "));
  });

  std::vector<std::string> drop_unique_keys_sql_operations;
  drop_unique_keys_sql_operations.reserve(params_.drop_unique_keys.size());
  std::for_each(begin(params_.drop_unique_keys), end(params_.drop_unique_keys), [&](const UniqueKey& key) {
    drop_unique_keys_sql_operations.push_back(DropUniqueKeyToPostgreSqlString(key));
  });

  std::vector<std::string> add_unique_keys_sql_operations;
  add_unique_keys_sql_operations.reserve(params_.add_unique_keys.size());
  std::for_each(begin(params_.add_unique_keys), end(params_.add_unique_keys), [&](const UniqueKey& key) {
    add_unique_keys_sql_operations.push_back(AddUniqueKeyToPostgreSqlString(key));
  });

  std::vector<std::string> sql_operations;
  sql_operations.push_back(boost::algorithm::join(drop_column_sql_operations, '\n'));
  sql_operations.push_back(boost::algorithm::join(add_column_sql_operations, ",\n"));
  sql_operations.push_back(boost::algorithm::join(add_foreign_keys_sql_operations, ",\n"));
  sql_operations.push_back(boost::algorithm::join(drop_unique_keys_sql_operations, ",\n"));
  sql_operations.push_back(boost::algorithm::join(add_unique_keys_sql_operations, ",\n"));

  return fmt::format("ALTER TABLE {:s} {:s}", params_.table_name, boost::algorithm::join(sql_operations, ','));
}

}