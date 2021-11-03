#include "listener.h"

#include "exceptions.h"
#include "session.h"

namespace core {

Listener::Listener(boost::asio::io_context& io_context, boost::asio::ip::tcp::endpoint endpoint, std::shared_ptr<IHttpHandlerRegistry> http_handler_registry)
    : io_context_{io_context},
      acceptor_{boost::asio::make_strand(io_context_)},
      http_handler_registry_{std::move(http_handler_registry)} {
  boost::beast::error_code error;
  acceptor_.open(endpoint.protocol(), error);

  if (error) {
    throw HttpServerInitializationError{error.message()};
  }

  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), error);

  if (error) {
    throw HttpServerInitializationError{"Allowing address reuse error: " + error.message()};
  }

  acceptor_.bind(endpoint, error);

  if (error) {
    throw HttpServerInitializationError{"Bind error: " + error.message()};
  }

  acceptor_.listen(boost::asio::socket_base::max_listen_connections, error);

  if (error) {
    throw HttpServerInitializationError{"Start listening error: " + error.message()};
  }
}

void Listener::Run() { DoAccept(); }

void Listener::DoAccept() {
  acceptor_.async_accept(boost::asio::make_strand(io_context_), boost::beast::bind_front_handler(&Listener::OnAccept, shared_from_this()));
}

void Listener::OnAccept(boost::beast::error_code error, boost::asio::ip::tcp::socket socket) {
  if (error) {
    SPDLOG_ERROR("Accept error: {:s}", error.message());
  } else {
    // Create the session and run it
    std::make_shared<Session>(std::move(socket), http_handler_registry_)->Run();
  }

  // Accept another connection
  DoAccept();
}

}// namespace core
