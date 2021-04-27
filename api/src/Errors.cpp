#include "Errors.h"

namespace {

using namespace api;

class DbProxyErrorCategory final : public std::error_category {
public:
  auto name() const noexcept -> const char* override {
    return "DbProxyErrorCategory";
  }

  auto message(int code) const -> std::string override {
    switch (static_cast<DbProxyError>(code)) {
      case DbProxyError::kCommandLineParsingError: {
        return "Command line parsing error";
      }
      default: {
        return "Undefined error code";
      }
    }
  }
};

}

namespace api {

auto MakeErrorCode(DbProxyError code) noexcept -> std::error_code {
  static auto category = DbProxyErrorCategory{};
  return std::error_code{ static_cast<int>(code), category };
}

}
