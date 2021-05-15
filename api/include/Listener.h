#pragma once

namespace api {

class Listener : public std::enable_shared_from_this<Listener> {
public:
  Listener(boost::asio::io_context& io_context, boost::asio::ip::tcp::endpoint endpoint);

  void Run();

private:
  void DoAccept();
  void OnAccept(boost::beast::error_code error, boost::asio::ip::tcp::socket socket);

private:
  boost::asio::io_context& io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

};