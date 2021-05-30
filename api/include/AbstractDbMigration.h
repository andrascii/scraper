#pragma once

#include "IDbMigration.h"
#include "PgConnectionFactory.h"

namespace api {

class AbstractDbMigration : public IDbMigration {
public:
  AbstractDbMigration(std::string migration_id);

  void ExecuteIfNeeded(const SharedPgConnection& connection) const override;

private:
  void MarkAsApplied(const SharedPgConnection& connection) const;
  bool IsAlreadyApplied(const SharedPgConnection& connection, const std::string& migration_id) const override;

private:
  std::string migration_id_;
};

}
