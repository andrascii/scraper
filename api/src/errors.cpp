#include "errors.h"

namespace {

using namespace api;

class DbProxyErrorCategory final : public std::error_category {
public:
  [[nodiscard]] const char* name() const noexcept override {
    return "DbProxyErrorCategory";
  }

  [[nodiscard]] std::string message(int code) const override {
    switch (static_cast<Error>(code)) {
      case Error::kCommandLineParsingError: {
        return "Command line parsing error";
      }
      case Error::kHttpServerInitializationFailed: {
        return "HTTP server initialization failed";
      }
      case Error::kUndefinedPostHttpHandler: {
        return "Undefined POST HTTP handler type";
      }
      case Error::kUndefinedGetHttpHandler: {
        return "Undefined GET HTTP handler type";
      }
      case Error::kNotFoundTypeFieldInReceivedRequest: {
        return "Not found 'type' field in the received HTTP request";
      }
      case Error::kUnknownTypeFieldValueInReceivedRequest: {
        return "Received value of field 'type' is unknown";
      }
      case Error::kNotFoundAllRequiredFieldsInAddJobRequest: {
        return "Not found all required fields in AddJobRequest JSON";
      }
      default: {
        return "Undefined error code";
      }
    }
  }
};

}

namespace api {

auto MakeErrorCode(Error code) noexcept -> std::error_code {
  static auto category = DbProxyErrorCategory{};
  return std::error_code{ static_cast<int>(code), category };
}

auto MakeErrorCode(boost::system::error_code error) noexcept -> std::error_code {
  class CategoryAdapter final : public std::error_category {
   public:
    explicit CategoryAdapter(const boost::system::error_category& category)
      : category_(category) {}

    [[nodiscard]] const char* name() const noexcept override {
      return category_.name();
    }

    [[nodiscard]] std::string message(int code) const override {
      return category_.message(code);
    }

   private:
    const boost::system::error_category& category_;
  };

  static thread_local std::unordered_map<std::string, CategoryAdapter> name_to_category;
  [[maybe_unused]] const auto& [iterator, is_inserted] = name_to_category.emplace(error.category().name(), error.category());
  [[maybe_unused]] const auto& [key, value] = *iterator;
  return std::error_code{ error.value(), value };
}

}
