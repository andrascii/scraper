#pragma once

#include "idb_migration.h"
#include "helpers.h"

namespace core {

class IMigrationFactory {
 public:
  using ExpectedMigration = common::Expected<std::shared_ptr<IDbMigration>>;

  enum Database {
    kScraper
  };

  virtual ~IMigrationFactory() = default;
  virtual ExpectedMigration Create(Database db) const = 0;
};

}