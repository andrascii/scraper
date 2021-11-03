#pragma once

namespace core {

using namespace boost::beast;

template <class Body, class Allocator>
using RequestType = http::request<Body, http::basic_fields<Allocator>>;

using ResponseType = http::response<http::string_body>;

enum class ContentType {
  kTextHtml,
  kTextCss,
  kTextJavaScript,
  kTextPlain,
  kApplicationOctetStream,
};

inline string_view ToString(ContentType type) noexcept {
  switch (type) {
    case ContentType::kTextHtml:
      return "text/html";
    case ContentType::kTextCss:
      return "text/css";
    case ContentType::kTextJavaScript:
      return "text/javascript";
    case ContentType::kTextPlain:
      return "text/plain";
    case ContentType::kApplicationOctetStream:
      return "application/octet-stream";
    default:
      return "content/unknown";
  }
}

inline void ConfigureResponse(ResponseType& response, string_view body, ContentType type) {
  response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  response.set(http::field::content_type, ToString(type));
  response.keep_alive(false);
  response.body() = std::string{body};
  response.prepare_payload();
}

inline auto BadRequest(unsigned request_version, string_view body, ContentType type) {
  ResponseType response{http::status::bad_request, request_version};

  ConfigureResponse(response, body, type);
  return response;
}

inline auto Ok(unsigned request_version, string_view body, ContentType type) {
  ResponseType response{http::status::ok, request_version};

  ConfigureResponse(response, body, type);
  return response;
}

}// namespace core