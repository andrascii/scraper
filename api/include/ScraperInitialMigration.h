#pragma once

#include "AbstractDbMigration.h"
#include "PgConnectionFactory.h"

namespace api {

class ScraperInitialMigration : public AbstractDbMigration {
 public:
  ScraperInitialMigration();

 private:
  void Execute(const SharedPgConnection& connection) const override;
  [[nodiscard]] bool IsAlreadyApplied(const SharedPgConnection& connection, const std::string& migration_id) const override;
};

}