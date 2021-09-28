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
  [[nodiscard]] static MigrationPointer WebsitesTableMigration();
  [[nodiscard]] static MigrationPointer ExtractionRulesTableMigration();
  [[nodiscard]] static MigrationPointer UrlStoreTableMigration();
  [[nodiscard]] static MigrationPointer LoadersTableMigration();
  [[nodiscard]] static MigrationPointer WebsitesSitemapColumnsMigration();
  [[nodiscard]] static MigrationPointer UrlStoreSitemapColumnsMigration();
  [[nodiscard]] static MigrationPointer WebsitesEntryPointsColumnMigration();
  [[nodiscard]] static MigrationPointer UrlStoreDropIsSitemapColumnMigration();
  [[nodiscard]] static MigrationPointer UrlStoreTypeColumnsMigration();
  [[nodiscard]] static MigrationPointer WebsitesPaginationNextXpathColumnMigration();
  [[nodiscard]] static MigrationPointer UrlStoreUniquePathWithTypeMigration();
  [[nodiscard]] static MigrationPointer ReportJobsCreateTableMigration();
  [[nodiscard]] static MigrationPointer ReportResultsCreateTableMigration();
  [[nodiscard]] static MigrationPointer WebsitesPaginationRulesColumnMigration();
  [[nodiscard]] static MigrationPointer WebsitesDropPaginationNextXpathColumnMigration();
  [[nodiscard]] static MigrationPointer WebsitesMatchMasksColumnMigration();
};

}