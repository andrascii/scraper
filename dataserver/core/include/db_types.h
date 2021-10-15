#pragma once

namespace core {

enum class DatabaseColumnType {
  kInt,
  kBigInt,
  kText,
  kBlob,
  kBoolean,
  kTimestamp,
  kVarChar256,
};

struct NullType {};
constexpr NullType kNull;

using DefaultValueType = std::variant<
  std::monostate, // used to indicate that the variant has no value
  bool,
  NullType,
  std::string,
  int64_t,
  uint64_t,
  double
>;

struct UniqueKey {
  std::vector<std::string> columns;
};

struct ForeignKey {
  std::vector<std::string> columns;
  std::string foreign_table;
  std::vector<std::string> foreign_columns;
};

struct Reference {
  std::string table_name;
  std::string column_name;
};

struct ColumnDefinition {
  std::string name;
  DatabaseColumnType column_type;
  DefaultValueType default_value;
  std::optional<Reference> reference;
  bool unique{ false };
  bool not_null{ false };
  bool primary_key{ false };
  bool auto_increment{ false };
};

}
