#pragma once

#include "helpers.h"
#include "idb_migration.h"

namespace core {

class IMigrationFactory {
 public:

  using ExpectedMigration = common::Expected<std::shared_ptr<IDbMigration>>;

  enum Database { kScraper };

  virtual ~IMigrationFactory() = default;
  virtual ExpectedMigration Create(Database db) const = 0;
};

}// namespace core