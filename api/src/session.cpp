#include "session.h"
#include "http_response_misc.h"

namespace api {

Session::Session(
  ba::ip::tcp::socket&& socket,
  std::shared_ptr<IHttpHandlerRegistry> http_handler_registry
) : stream_{std::move(socket)},
    lambda_{*this},
    http_handler_registry_{std::move(http_handler_registry)} {}

void Session::Run() {
  // We need to be executing within a strand to perform async operations
  // on the I/O objects in this session. Although not strictly necessary
  // for single-threaded contexts, this example code is written to be
  // thread-safe by default.
  dispatch(
    stream_.get_executor(),
    bind_front_handler(&Session::DoRead, shared_from_this())
  );
}

void Session::DoRead() {
  // Make the request empty before reading,
  // otherwise the operation behavior is undefined.
  request_ = {};

  // Set the timeout.
  stream_.expires_after(30s);

  // Read a request
  async_read(
    stream_,
    buffer_,
    request_,
    bind_front_handler(&Session::OnRead, shared_from_this())
  );

  SPDLOG_TRACE("Reading request...");
}

void Session::OnRead(error_code error, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  // This means they closed the connection
  if (error == http::error::end_of_stream) {
    return DoClose();
  }

  if (error) {
    SPDLOG_ERROR("Reading error: {:s}", error.message());
    return;
  }

  SPDLOG_TRACE(
    "incoming HTTP request: {}",
    request_
  );

  const auto header = request_.base();
  const auto method = header.method();
  const auto request_version = request_.version();
  const auto method_string = header.method_string();

  const auto target_view = header.target();
  const auto target = std::string{target_view.data(), target_view.length()};

  if (method == http::verb::post) {
    const auto handler_name = target.substr(1, target.find('?'));
    const auto expected_type = ToPostHttpHandlerType(handler_name);

    if (!expected_type) {
      SPDLOG_WARN("not found requested HTTP handler: {}", handler_name);

      lambda_(
        BadRequest(
          request_.version(),
          "not found requested HTTP handler",
          ContentType::kTextHtml
        ));

      return;
    }

    auto expected_response = http_handler_registry_->HandleRequest(*expected_type, std::move(request_));

    if (!expected_response) {
      lambda_(
        BadRequest(
          request_version,
          expected_response.error().message(),
          ContentType::kTextPlain
        ));
    } else {
      lambda_(std::move(*expected_response));
    }
  } else {
    SPDLOG_WARN("request with unsupported HTTP method: {}", method_string);
    lambda_(
      BadRequest(
        request_version,
        "Unsupported HTTP method",
        ContentType::kTextHtml
      ));
  }
}

void Session::OnWrite(bool close, error_code error, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (error) {
    SPDLOG_ERROR("Write error: {:s}", error.message());
    return;
  }

  if (close) {
    // This means we should close the connection, usually because
    // the response indicated the "Descriptor: close" semantic.
    return DoClose();
  }

  SPDLOG_TRACE("Response successfully sent");

  // We're done with the response so delete it
  response_ = nullptr;

  // Read another request
  DoRead();
}

void Session::DoClose() {
  SPDLOG_TRACE("Descriptor closed");

  // Send a TCP shutdown
  error_code error;
  stream_.socket().shutdown(ba::ip::tcp::socket::shutdown_send, error);

  // At this point the connection is closed gracefully
}

}