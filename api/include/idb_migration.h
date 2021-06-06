#pragma once

#include "variant_visitor.h"
#include "pg_connection_factory.h"

namespace api {

class IDbMigration {
 public:
  virtual ~IDbMigration() = default;

  virtual const std::string& MigrationId() const noexcept = 0;
  virtual void ExecuteIfNeeded(const std::shared_ptr<pqxx::connection>& connection) const = 0;

 protected:
  //
  // Override it in your own implementation
  //
  virtual void Execute(const std::shared_ptr<pqxx::connection>& connection) const = 0;

  [[nodiscard]] virtual bool IsAlreadyApplied(
    const std::shared_ptr<pqxx::connection>& connection,
    const std::string& migration_id
  ) const = 0;
};

}
