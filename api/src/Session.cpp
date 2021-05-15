#include "Session.h"

namespace {

template <class Body, class Allocator>
using RequestType = boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>;

template <class Body, class Allocator>
auto BadRequest(RequestType<Body, Allocator>&& request, boost::beast::string_view why) {
  boost::beast::http::response<boost::beast::http::string_body> response{
    boost::beast::http::status::bad_request,
    request.version()
  };

  response.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
  response.set(boost::beast::http::field::content_type, "text/plain");
  response.keep_alive(false);
  response.body() = std::string{ why };
  response.prepare_payload();
  return response;
}

}

namespace api {

Session::Session(boost::asio::ip::tcp::socket&& socket)
  : stream_{ std::move(socket) },
    lambda_{ *this } {}

void Session::Run() {
  // We need to be executing within a strand to perform async operations
  // on the I/O objects in this session. Although not strictly necessary
  // for single-threaded contexts, this example code is written to be
  // thread-safe by default.
  dispatch(stream_.get_executor(), boost::beast::bind_front_handler(&Session::DoRead, shared_from_this()));
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
    boost::beast::bind_front_handler(&Session::OnRead, shared_from_this()));

  SPDLOG_TRACE("Reading request...");
}

void Session::OnRead(boost::beast::error_code error, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  // This means they closed the connection
  if (error == boost::beast::http::error::end_of_stream) {
    return DoClose();
  }

  if (error) {
    SPDLOG_ERROR("Reading error: {:s}", error.message());
    return;
  }

  // Send the response
  lambda_(BadRequest(std::move(request_), "First test"));
}

void Session::OnWrite(bool close, boost::beast::error_code error, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (error) {
    SPDLOG_ERROR("Write error: {:s}", error.message());
    return;
  }

  if (close) {
    // This means we should close the connection, usually because
    // the response indicated the "Connection: close" semantic.
    return DoClose();
  }

  SPDLOG_TRACE("Response successfully sent");

  // We're done with the response so delete it
  res_ = nullptr;

  // Read another request
  DoRead();
}

void Session::DoClose() {
  SPDLOG_TRACE("Connection closed");

  // Send a TCP shutdown
  boost::beast::error_code error;
  stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, error);

  // At this point the connection is closed gracefully
}

}