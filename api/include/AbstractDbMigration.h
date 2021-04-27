#pragma once

#include "IDbMigration.h"

namespace api {

class AbstractDbMigration : public IDbMigration {
public:
  void executeIfNeeded() const override;

private:
  
};

}
