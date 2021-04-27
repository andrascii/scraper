#pragma once

#include "DbTypes.h"

namespace api {

constexpr char* ToString(DatabaseColumnType type) noexcept {
  switch (type) {
    case kInt: return "INTEGER";
    case kBigInt: return "BIGINT";
    case kText: return "TEXT";
    case kBlob: return "BYTEA";
    case kBoolean: return "boolean";
    case kTimestamp: return "timestamp without time zone";
    case kVarchar256: return "character varying(256)";
  }

  abort();
}

constexpr QString DefaultValueToPostgreSqlString(const DefaultValueType& value) {
  common::VariantVisitor visitor{
    [](bool value) { return value ? "true" : "false"; },
    [](NullType) { return "null"; },
    [](int64_t value) { return QString::number(value); }
    [](uint64_t value) { return QString::number(value); },
    [](double value) { return QString::number(value); },
    [](const std::string& value) { return QString::fromStdString(value); },
    [](const QString& value) { return value; }
  };

  return std::visit(visitor, value);
}

inline QString ColumnToPostgreSqlString(const ColumnDefinition& column, const QString& prefix = "") {
  QStringList lst;

  lst
    << prefix
    << column.name
    << column.auto_increment ? "BIGSERIAL" : ToString(column.column_type)
    << column.not_null ? "NUT NULL" : ""
    << column.primary_key ? "PRIMARY KEY" : column.unique ? "UNIQUE" : ""
    << column.default_value.index() != std::variant_npos ? "DEFAULT " % DefaultValueToPostgreSqlString(column.default_value) : ""
    << column.reference.has_value() ? "REFERENCES " % column.reference->table_name % "(" % column.reference->column_name % ")" : "";

  return lst.join(" ");
}

inline QString DropColumnString(const QString& column_name) {
  return "DROP COLUMN " % column_name;
}

inline QString ForeignKeyToPostgreSqlString(const ForeignKey& foreign_key, const QString& prefix = "") {
  return prefix % "CONSTRAINT fk_" % foreign_key.columns.join("_") % " " %
    "FOREIGN KEY(" % foreign_key.columns.join(", ") % ") " %
    "REFERENCES " % foreign_key.foreign_table % "(" % foreign_key.foreign_columns.join(",") % ")";
}

inline QString UniqueKeyConstraintName(const QStringList& columns) {
  return "unique_" % columns.join("_");
}

inline QString AddUniqueKeyToPostgreSqlStringCreateTable(const QStringList& unique_key_columns) {
  return "CONSTRAINT " % UniqueKeyConstraintName(unique_key_columns) % " UNIQUE(" % unique_key_columns.join(", ") % ")";
}

inline QString AddUniqueKeyToPostgreSqlString(const QStringList& unique_key_columns) {
  return "ADD " % AddUniqueKeyToPostgreSqlStringCreateTable(unique_key_columns);
}

inline QString DropUniqueKeyToPostgreSqlString(const QStringList& unique_key_columns) {
  return "DROP CONSTRAINT " % UniqueKeyConstraintName(unique_key_columns);
}

}
