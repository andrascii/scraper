#pragma once

namespace api {

enum class DbProxyError {
  kCommandLineParsingError = 1,
  kHttpServerInitializationFailed
};

auto MakeErrorCode(DbProxyError code) noexcept -> std::error_code;
auto MakeErrorCode(boost::system::error_code error) noexcept -> std::error_code;

}

namespace std {

template <> struct is_error_code_enum<api::DbProxyError> : public true_type {};

}
