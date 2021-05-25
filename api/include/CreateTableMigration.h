#pragma once

#include "AbstractDbMigration.h"
#include "PgConnectionFactory.h"
#include "DbTypes.h"

namespace api {

class CreateTableMigration : public AbstractDbMigration {
 public:
  struct Params {
    std::string table_name;
    std::vector<ColumnDefinition> columns;
    std::vector<ForeignKey> foreign_keys;
    std::vector<UniqueKey> unique_keys;
  };

  CreateTableMigration(const std::string& migration_id, const SharedPgConnection& connection, Params params);

 private:
  void Execute() const override;

  [[nodiscard]] std::string BuildSqlQuery() const;

 private:
  Params params_;
  SharedPgConnection connection_;
};

}