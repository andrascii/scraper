#pragma once

#include "DbTypes.h"

namespace api {

constexpr const char* ToString(DatabaseColumnType type) noexcept {
  switch (type) {
    case DatabaseColumnType::kInt: return "INTEGER";
    case DatabaseColumnType::kBigInt: return "BIGINT";
    case DatabaseColumnType::kText: return "TEXT";
    case DatabaseColumnType::kBlob: return "BYTEA";
    case DatabaseColumnType::kBoolean: return "boolean";
    case DatabaseColumnType::kTimestamp: return "timestamp without time zone";
    case DatabaseColumnType::kVarChar256: return "character varying(256)";
  }

  abort();
}

constexpr const char* ToString(LinkType type) noexcept {
  switch (type) {
    case LinkType::kCommon: return "common";
    case LinkType::kSiteMap: return "sitemap";
    case LinkType::kEntryPoint: return "entry_point";
    case LinkType::kPagination: return "pagination";
  }

  abort();
}

inline std::string DefaultValueToPostgreSqlString(const DefaultValueType& value) {
  common::VariantVisitor visitor{
    [](bool value) { return value ? "true"s : "false"s; },
    [](NullType) { return "null"s; },
    [](const std::string& value) { return value; },
    [](int64_t value) { return std::to_string(value); },
    [](uint64_t value) { return std::to_string(value); },
    [](double value) { return std::to_string(value); },
    [](LinkType value) { return std::string{ToString(value)}; }
  };

  return std::visit(visitor, value);
}

inline std::string ColumnToPostgreSqlString(const ColumnDefinition& column, const std::string& prefix = "") {
  std::vector<std::string> lst;

  lst.push_back(prefix);
  lst.push_back(column.name);

  if (column.auto_increment) {
    lst.emplace_back("BIGSERIAL");
  } else {
    lst.emplace_back(ToString(column.column_type));
  }

  if (column.not_null) {
    lst.emplace_back("NOT NULL");
  } else {
    lst.emplace_back();
  }

  if (column.primary_key) {
    lst.emplace_back("PRIMARY KEY");
  } else if (column.unique) {
    lst.emplace_back("UNIQUE");
  } else {
    lst.emplace_back();
  };

  if (column.default_value.index() != std::variant_npos) {
    lst.push_back("DEFAULT " + DefaultValueToPostgreSqlString(column.default_value));
  } else {
    lst.emplace_back();
  }

  if (column.reference.has_value()) {
    lst.push_back("REFERENCES " + column.reference->table_name + "(" + column.reference->column_name + ")");
  } else {
    lst.emplace_back();
  }

  return boost::algorithm::join(lst, " ");
}

inline std::string DropColumnString(const std::string& column_name) {
  return "DROP COLUMN " + column_name;
}

inline std::string ForeignKeyToPostgreSqlString(const ForeignKey& foreign_key, const std::string& prefix = "") {
  return
    prefix + "CONSTRAINT fk_" + boost::algorithm::join(foreign_key.columns, "_") + " " +
    "FOREIGN KEY(" + boost::algorithm::join(foreign_key.columns, ", ") + ") " +
    "REFERENCES " + foreign_key.foreign_table + "(" + boost::algorithm::join(foreign_key.foreign_columns, ",") + ")";
}

inline std::string UniqueKeyConstraintName(const UniqueKey& unique_key) {
  return "unique_" + boost::algorithm::join(unique_key.columns, "_");
}

inline std::string AddUniqueKeyToPostgreSqlStringCreateTable(const UniqueKey& unique_key) {
  return
    "CONSTRAINT " +
    UniqueKeyConstraintName(unique_key) +
    " UNIQUE(" + boost::algorithm::join(unique_key.columns, ", ") +
    ")";
}

inline std::string AddUniqueKeyToPostgreSqlString(const UniqueKey& unique_key) {
  return "ADD " + AddUniqueKeyToPostgreSqlStringCreateTable(unique_key);
}

inline std::string DropUniqueKeyToPostgreSqlString(const UniqueKey& unique_key) {
  return "DROP CONSTRAINT " + UniqueKeyConstraintName(unique_key);
}

}
