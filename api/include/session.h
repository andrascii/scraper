#pragma once

namespace api {

class Session : public std::enable_shared_from_this<Session> {
 public:
  explicit Session(boost::asio::ip::tcp::socket&& socket);

  void Run();

 private:
  void DoRead();
  void OnRead(boost::beast::error_code error, std::size_t bytes_transferred);

  void OnWrite(bool close, boost::beast::error_code error, std::size_t bytes_transferred);
  void DoClose();

 private:
  // This is the C++11 equivalent of a generic lambda.
  // The function object is used to send an HTTP message.
  struct SendLambda {
    Session& self_;

    explicit SendLambda(Session& self) : self_(self) {}

    template<bool isRequest, class Body, class Fields>
    void operator()(boost::beast::http::message<isRequest, Body, Fields>&& msg) const {
      // The lifetime of the message has to extend
      // for the duration of the async operation so
      // we use a shared_ptr to manage it.
      auto sp = std::make_shared<boost::beast::http::message<isRequest, Body, Fields>>(std::move(msg));

      // Store a type-erased version of the shared
      // pointer in the class to keep it alive.
      self_.response_ = sp;

      // Write the response
      async_write(
        self_.stream_,
        *sp,
        boost::beast::bind_front_handler(&Session::OnWrite, self_.shared_from_this(), sp->need_eof()));

      SPDLOG_TRACE("Sending response");
    }
  };

  boost::beast::tcp_stream stream_;
  boost::beast::flat_buffer buffer_;
  boost::beast::http::request<boost::beast::http::string_body> request_;
  std::shared_ptr<void> response_;
  SendLambda lambda_;
};

}