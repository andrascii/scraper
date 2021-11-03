#pragma once

#include "abstract_db_migration.h"
#include "db_types.h"
#include "pg_connection_factory.h"

namespace core {

class CreateTableMigration : public AbstractDbMigration {
 public:

  struct Params {
    std::string table_name;
    std::vector<ColumnDefinition> columns;
    std::vector<ForeignKey> foreign_keys;
    std::vector<UniqueKey> unique_keys;
  };

  CreateTableMigration(const std::string& migration_id, Params params);

 private:

  void Execute(const std::shared_ptr<pqxx::connection>& connection) const override;

  [[nodiscard]] std::string BuildSqlQuery() const;

 private:

  Params params_;
};

}// namespace core