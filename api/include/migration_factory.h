#pragma once

#include "imigration_factory.h"
#include "abstract_db_migration.h"
#include "not_null.h"

namespace api {

class MigrationFactory : public IMigrationFactory {
 public:
  [[nodiscard]] ExpectedMigration Create(Database db) const override;

 private:
  using MigrationPointer = common::NotNull<std::shared_ptr<IDbMigration>>;

  [[nodiscard]] static ExpectedMigration ScraperMigration() noexcept;
  [[nodiscard]] static MigrationPointer InitialMigration();
  [[nodiscard]] static MigrationPointer JobsTableMigration();
  [[nodiscard]] static MigrationPointer ActionsTableMigration();
  [[nodiscard]] static MigrationPointer LoadersTableMigration();
  [[nodiscard]] static MigrationPointer ReportJobsCreateTableMigration();
  [[nodiscard]] static MigrationPointer ReportResultsCreateTableMigration();
};

}