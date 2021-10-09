#pragma once

namespace api {

enum class Error {
  kCommandLineParsingError = 1,
  kHttpServerInitializationFailed,
  kUndefinedPostHttpHandler,
  kUndefinedGetHttpHandler,
  kNotFoundTypeFieldInReceivedRequest,
  kUnknownTypeFieldValueInReceivedRequest,
  kNotFoundAllRequiredFieldsInAddJobRequest,
  kJsonIsNotAnArray,
  kJsonIsNotAnObject,
};

auto MakeErrorCode(Error code) noexcept -> std::error_code;
auto MakeErrorCode(boost::system::error_code error) noexcept -> std::error_code;

}

namespace std {

template <> struct is_error_code_enum<api::Error> : public true_type {};

}
