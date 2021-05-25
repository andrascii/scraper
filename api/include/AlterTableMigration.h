#pragma once

#include "AbstractDbMigration.h"
#include "DbTypes.h"

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

  AlterTableMigration(const std::string& migration_id, const SharedPgConnection& connection, Params params);

 private:
  void Execute() const override;

  [[nodiscard]] std::string BuildSqlQuery() const;

 private:
  Params params_;
  SharedPgConnection connection_;
};

}