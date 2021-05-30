#pragma once

#include "VariantVisitor.h"
#include "PgConnectionFactory.h"

namespace api {

class IDbMigration {
 public:
  virtual ~IDbMigration() = default;
  virtual void ExecuteIfNeeded(const SharedPgConnection& connection) const = 0;

 protected:
  //
  // Override it in your own implementation
  //
  virtual void Execute(const SharedPgConnection& connection) const = 0;
  [[nodiscard]] virtual bool IsAlreadyApplied(const SharedPgConnection& connection, const std::string& migration_id) const = 0;
};

}
