#pragma once

namespace api {

enum class DatabaseColumnType {
  kInt,
  kBigInt,
  kText,
  kBlob,
  kBoolean,
  kTimestamp,
  kVarChar256,
};

enum class LinkType {
  kCommon,
  kSiteMap,
  kEntryPoint,
  kPagination,
};

struct NullType {};
inline NullType Null;

using DefaultValueType = std::variant<
  bool,
  NullType,
  std::string,
  int64_t,
  uint64_t,
  double,
  LinkType
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
