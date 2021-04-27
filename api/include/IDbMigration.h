#pragma once

#include "VariantVisitor.h"

namespace api {

class IDbMigration {
public:
  virtual ~IDbMigration() = default;
  virtual void executeIfNeeded() const = 0;

private:
  //
  // Override it in your own implementation
  //
  virtual void execute() const = 0;
};

}
