#include "migration_factory.h"
#include "alter_table_migration.h"
#include "create_table_migration.h"
#include "helpers.h"
#include "scraper_initial_migration.h"

namespace api {

MigrationFactory::ExpectedMigration MigrationFactory::Create(Database db) const {
  switch (db) {
    case kScraper: {
      return ScraperMigration();
    }
    default: {
      SPDLOG_ERROR("Cannot create migration cause of undefined database type passed");

      return common::Unexpected<>{
        std::make_error_code(std::errc::invalid_argument)
      };
    }
  }
}

MigrationFactory::ExpectedMigration MigrationFactory::ScraperMigration() noexcept {
  try {
    const auto migration = InitialMigration();

    migration
      ->AddNext(JobsTableMigration())
      ->AddNext(ActionsTableMigration())
      ->AddNext(LoadersTableMigration())
      ->AddNext(ReportJobsCreateTableMigration())
      ->AddNext(ReportResultsCreateTableMigration());

    return migration;
  } catch (const std::exception& ex) {
    SPDLOG_ERROR("{}", ex.what());
    return common::Unexpected<>{
      std::make_error_code(std::errc::operation_canceled)
    };
  }
}

MigrationFactory::MigrationPointer MigrationFactory::InitialMigration() {
  return common::MakeNotNull(std::make_shared<ScraperInitialMigration>());
}

MigrationFactory::MigrationPointer MigrationFactory::JobsTableMigration() {
  return common::MakeNotNull(
    std::make_shared<CreateTableMigration>(
      "jobs_table",
      CreateTableMigration::Params{
        .table_name = "jobs",
        .columns = {
          {.name = "id", .column_type = DatabaseColumnType::kBigInt, .primary_key = true, .auto_increment = true},
          {.name = "update_frequency", .column_type = DatabaseColumnType::kBigInt, .not_null = true},
          {.name = "enabled", .column_type = DatabaseColumnType::kBoolean, .not_null = true},
        }
      }
    )
  );
}

MigrationFactory::MigrationPointer MigrationFactory::ActionsTableMigration() {
  return common::MakeNotNull(
    std::make_shared<CreateTableMigration>(
      "actions_table",
      CreateTableMigration::Params{
        .table_name = "actions",
        .columns = {
          {.name = "id", .column_type = DatabaseColumnType::kBigInt, .primary_key = true, .auto_increment = true},
          {.name = "job_id", .column_type = DatabaseColumnType::kBigInt, .reference = Reference{"jobs", "id"}, .not_null = false},
          {.name = "action_json", .column_type = DatabaseColumnType::kText, .not_null = true},
        }
      }
    )
  );
}

MigrationFactory::MigrationPointer MigrationFactory::LoadersTableMigration() {
  return common::MakeNotNull(
    std::make_shared<CreateTableMigration>(
      "loaders_table",
      CreateTableMigration::Params{
        .table_name = "loaders",
        .columns = {
          {.name = "id", .column_type = DatabaseColumnType::kBigInt, .primary_key = true, .auto_increment = true},
          {.name = "mac_address", .column_type = DatabaseColumnType::kBigInt, .unique = true, .not_null = true},
        },
      }
    )
  );
}

MigrationFactory::MigrationPointer MigrationFactory::ReportJobsCreateTableMigration() {
  return common::MakeNotNull(
    std::make_shared<CreateTableMigration>(
      "report_jobs_create_table",
      CreateTableMigration::Params{
        .table_name = "report_jobs",
        .columns = {
          {.name = "id", .column_type = DatabaseColumnType::kBigInt, .primary_key = true, .auto_increment = true},
          {.name = "report_type", .column_type = DatabaseColumnType::kVarChar256, .default_value = false, .not_null = false},
          {.name = "report_info", .column_type = DatabaseColumnType::kText, .not_null = true},
          {.name = "job_id", .column_type = DatabaseColumnType::kBigInt, .reference = Reference{"jobs", "id"}, .not_null = false},
          {.name = "status", .column_type = DatabaseColumnType::kBigInt, .default_value = uint64_t{}, .not_null = true},
          {.name = "last_change_timestamp", .column_type = DatabaseColumnType::kTimestamp, .not_null = true},
        },
      }
    )
  );
}

MigrationFactory::MigrationPointer MigrationFactory::ReportResultsCreateTableMigration() {
  return common::MakeNotNull(
    std::make_shared<CreateTableMigration>(
      "report_results_create_table",
      CreateTableMigration::Params{
        .table_name = "report_results",
        .columns = {
          {.name = "report_job_id", .column_type = DatabaseColumnType::kBigInt, .reference = Reference{"report_jobs", "id"}},
          {.name = "result", .column_type = DatabaseColumnType::kText, .not_null = true},
        },
      }
    )
  );
}

}// namespace api