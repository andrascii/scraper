#pragma once

#include "AbstractDbMigration.h"
#include "PgConnectionFactory.h"

namespace api {

class CreateDatabaseMigration : public AbstractDbMigration {
 public:
  CreateDatabaseMigration(SharedPgConnection connection, std::string database_name);

 private:
  void Execute() const override;
  [[nodiscard]] bool IsAlreadyApplied(const std::string& migration_id) const override;

 private:
  std::string database_name_;
  SharedPgConnection connection_;
};

}