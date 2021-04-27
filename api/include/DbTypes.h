#pragma once

namespace api {

enum class DatabaseColumnType {
  kInt,
  kBigInt,
  kText,
  kBlob,
  kBoolean,
  kTimestamp,
  kVarchar256,
};

struct NullType {} Null;

using DefaultValueType = std::variant<bool, NullType, std::string, QString, int64_t, uint64_t, double>;

struct ForeignKey {
  QStringList columns;
  QString foreign_table;
  QStringList foreign_columns;
};

struct Reference {
  QString table_name;
  QString column_name;
};

struct ColumnDefinition {
  QString name;
  DatabaseColumnType column_type;
  DefaultValueType default_value;
  std::optional<Reference> reference;
  bool unique{ false };
  bool not_null{ false };
  bool primary_key{ false };
  bool auto_increment{ false };
};

}
