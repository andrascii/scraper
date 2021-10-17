#pragma once

#include "abstract_db_migration.h"
#include "db_types.h"

namespace core {

class AlterTableMigration : public AbstractDbMigration {
 public:
  struct Params {
    std::string table_name;
    std::vector<ColumnDefinition> add_columns; // columns to add to specified table
    std::vector<std::string> drop_columns; // column names to drop from specified table
    std::vector<ForeignKey> add_foreign_keys; // foreign keys to add
    std::vector<UniqueKey> add_unique_keys; // unique keys to add
    std::vector<UniqueKey> drop_unique_keys; // unique keys to drop
  };

  AlterTableMigration(const std::string& migration_id, Params params);

 private:
  void Execute(const std::shared_ptr<pqxx::connection>& connection) const override;

  [[nodiscard]] std::string BuildSqlQuery() const;

 private:
  Params params_;
};

}