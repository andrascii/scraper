#include "ScraperMigrator.h"
#include "AlterTableMigration.h"
#include "CreateTableMigration.h"
#include "ScraperInitialMigration.h"

namespace api {

ScraperMigrator::ScraperMigrator(SharedSettings settings, std::shared_ptr<PgConnectionPool> pool)
  : settings_{std::move(settings)},
    pool_{std::move(pool)} {
  migrations_.emplace_back(std::make_unique<ScraperInitialMigration>());

  migrations_.emplace_back(std::make_unique<CreateTableMigration>("websites_table", CreateTableMigration::Params{
    .table_name = "websites",
    .columns = {
      {.name = "id", .column_type = DatabaseColumnType::kBigInt, .primary_key = true, .auto_increment = true},
      {.name = "address", .column_type = DatabaseColumnType::kVarChar256, .unique = true, .not_null = true},
      {.name = "update_frequency", .column_type = DatabaseColumnType::kBigInt, .not_null = true},
      {.name = "enabled", .column_type = DatabaseColumnType::kBoolean, .not_null = true},
      {.name = "ignore_masks", .column_type = DatabaseColumnType::kText},
      {.name = "parse_queries", .column_type = DatabaseColumnType::kBoolean, .default_value = true, .not_null = true},
      {.name = "parse_anchors", .column_type = DatabaseColumnType::kBoolean, .default_value = true, .not_null = true},
    }
  }));

  migrations_.emplace_back(std::make_unique<CreateTableMigration>("extraction_rules_table", CreateTableMigration::Params{
    .table_name = "extraction_rules",
    .columns = {
      {.name = "id", .column_type = DatabaseColumnType::kBigInt, .primary_key = true, .auto_increment = true},
      {.name = "website_id", .column_type = DatabaseColumnType::kBigInt, .not_null = true},
      {.name = "update_frequency", .column_type = DatabaseColumnType::kBigInt, .not_null = true},
    }
  }));

  migrations_.emplace_back(std::make_unique<CreateTableMigration>("url_store_table", CreateTableMigration::Params{
    .table_name = "url_store",
    .columns = {
      {.name = "id", .column_type = DatabaseColumnType::kBigInt, .primary_key = true, .auto_increment = true},
      {.name = "path", .column_type = DatabaseColumnType::kText, .not_null = true},
      {.name = "taken_to_loading_time", .column_type = DatabaseColumnType::kTimestamp},
      {.name = "last_update_time", .column_type = DatabaseColumnType::kTimestamp},
      {.name = "found_data", .column_type = DatabaseColumnType::kBoolean},
      {.name = "website_id", .column_type = DatabaseColumnType::kBigInt, .reference = Reference{"websites", "id"}, .not_null = true},
      {.name = "loader_id", .column_type = DatabaseColumnType::kBigInt},
      {.name = "parsed_data", .column_type = DatabaseColumnType::kText},
      {.name = "unique_id", .column_type = DatabaseColumnType::kText},
    },
    .unique_keys = {
      {{"website_id", "path"}},
      {{"website_id", "id"}},
    }
  }));

  migrations_.emplace_back(std::make_unique<CreateTableMigration>("loaders_table", CreateTableMigration::Params{
    .table_name = "loaders",
    .columns = {
      {.name = "id", .column_type = DatabaseColumnType::kBigInt, .primary_key = true, .auto_increment = true},
      {.name = "mac_address", .column_type = DatabaseColumnType::kBigInt, .unique = true, .not_null = true},
    },
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("websites_site_map_columns", AlterTableMigration::Params{
    .table_name = "websites",
    .add_columns = {
      {.name = "site_map_url", .column_type = DatabaseColumnType::kVarChar256},
      {.name = "get_links_from_site_map_only", .column_type = DatabaseColumnType::kBoolean, .default_value = false, .not_null = true},
    },
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("url_store_site_map_columns", AlterTableMigration::Params{
    .table_name = "url_store",
    .add_columns = {
      {.name = "is_site_map", .column_type = DatabaseColumnType::kBoolean, .default_value = false, .not_null = true},
    },
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("websites_entry_points_column", AlterTableMigration::Params{
    .table_name = "websites",
    .add_columns = {
      {.name = "entry_points", .column_type = DatabaseColumnType::kText},
    },
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("url_store_drop_is_site_map_column", AlterTableMigration::Params{
    .table_name = "url_store",
    .drop_columns = {"is_site_map"},
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("url_store_type_columns", AlterTableMigration::Params{
    .table_name = "url_store",
    .add_columns = {
      {.name = "type", .column_type = DatabaseColumnType::kVarChar256, .default_value = LinkType::kCommon, .not_null = true},
    },
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("websites_pagination_next_xpath_column", AlterTableMigration::Params{
    .table_name = "websites",
    .add_columns = {
      {.name = "pagination_next_xpath", .column_type = DatabaseColumnType::kText},
    },
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("url_store_unique_path_with_type", AlterTableMigration::Params{
    .table_name = "url_store",
    .add_unique_keys = {
      {{"website_id", "path", "type"}},
    },
    .drop_unique_keys = {
      {{"website_id", "path"}},
    }
  }));

  migrations_.emplace_back(std::make_unique<CreateTableMigration>("report_jobs_create_table", CreateTableMigration::Params{
    .table_name = "report_jobs",
    .columns = {
      {.name = "id", .column_type = DatabaseColumnType::kBigInt, .primary_key = true, .auto_increment = true},
      {.name = "report_type", .column_type = DatabaseColumnType::kVarChar256, .default_value = false, .not_null = false},
      {.name = "report_info", .column_type = DatabaseColumnType::kText, .not_null = true},
      {.name = "customer_website_id", .column_type = DatabaseColumnType::kBigInt, .reference = Reference{"websites", "id"}, .not_null = false},
      {.name = "status", .column_type = DatabaseColumnType::kBigInt, .default_value = uint64_t{}, .not_null = true},
      {.name = "last_change_timestamp", .column_type = DatabaseColumnType::kTimestamp, .not_null = true},
    },
  }));

  migrations_.emplace_back(std::make_unique<CreateTableMigration>("report_results_create_table", CreateTableMigration::Params{
    .table_name = "report_results",
    .columns = {
      {.name = "report_job_id", .column_type = DatabaseColumnType::kBigInt, .reference = Reference{"report_jobs", "id"}},
      {.name = "result", .column_type = DatabaseColumnType::kText, .not_null = true},
    },
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("websites_pagination_rules_column", AlterTableMigration::Params{
    .table_name = "websites",
    .add_columns = {
      {.name = "pagination_rules", .column_type = DatabaseColumnType::kText, .default_value = "[]"s, .not_null = true},
    },
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("websites_drop_pagination_next_xpath_column", AlterTableMigration::Params{
    .table_name = "websites",
    .drop_columns = {"pagination_next_xpath"},
  }));

  migrations_.emplace_back(std::make_unique<AlterTableMigration>("websites_match_masks_column", AlterTableMigration::Params{
    .table_name = "websites",
    .add_columns = {
      {.name = "match_masks", .column_type = DatabaseColumnType::kText},
    },
  }));
}

void ScraperMigrator::Apply() const {
  SPDLOG_INFO("Applying scraper migrations");

  const auto wrapper = pool_->Take();

  for (const auto& migration : migrations_) {
    migration->ExecuteIfNeeded(wrapper.Connection());
  }

  SPDLOG_INFO("Looks like all is ok");
}

}