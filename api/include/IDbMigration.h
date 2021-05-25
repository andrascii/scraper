#pragma once

#include "VariantVisitor.h"

namespace api {

class IDbMigration {
 public:
  virtual ~IDbMigration() = default;
  virtual void ExecuteIfNeeded() const = 0;

 protected:
  //
  // Override it in your own implementation
  //
  virtual void Execute() const = 0;
  [[nodiscard]] virtual bool IsAlreadyApplied(const std::string& migration_id) const = 0;
};

}
