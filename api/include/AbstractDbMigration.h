#pragma once

#include "IDbMigration.h"
#include "PgConnectionFactory.h"

namespace api {

class AbstractDbMigration : public IDbMigration {
public:
  AbstractDbMigration(std::string migration_id, SharedPgConnection connection);

  void ExecuteIfNeeded() const override;

private:
  void MarkAsApplied() const;
  bool IsAlreadyApplied(const std::string& migration_id) const override;

private:
  std::string migration_id_;
  SharedPgConnection connection_;
};

}
