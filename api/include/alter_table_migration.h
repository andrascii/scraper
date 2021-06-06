#pragma once

#include "abstract_db_migration.h"
#include "db_types.h"

namespace api {

class AlterTableMigration : public AbstractDbMigration {
 public:
  struct Params {
    std::string table_name;
    std::vector<ColumnDefinition> add_columns;
    std::vector<std::string> drop_columns;
    std::vector<ForeignKey> add_foreign_keys;
    std::vector<UniqueKey> add_unique_keys;
    std::vector<UniqueKey> drop_unique_keys;
  };

  AlterTableMigration(const std::string& migration_id, Params params);

 private:
  void Execute(const std::shared_ptr<pqxx::connection>& connection) const override;

  [[nodiscard]] std::string BuildSqlQuery() const;

 private:
  Params params_;
};

}