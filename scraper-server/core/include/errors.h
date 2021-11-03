#pragma once

namespace core {

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
  kInvalidTypeValue,
};

auto MakeErrorCode(Error code) noexcept -> std::error_code;
auto MakeErrorCode(boost::system::error_code error) noexcept -> std::error_code;

}// namespace core

namespace std {

template <>
struct is_error_code_enum<core::Error> : public true_type {};

}// namespace std
