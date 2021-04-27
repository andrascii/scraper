#include "TcpClient.h"
#include "EchoTcpServer.h"

namespace dataapitests {

TEST(UnitTests, BoostClientServerEcho) {
  unsigned short port = 44444;
  dataapi::TcpClient client1;
  dataapi::TcpClient client2;
  dataapi::TcpClient client3;
  dataapi::EchoTcpServer server{ port };
  server.Start();

  boost::system::error_code error;
  const auto ip = boost::asio::ip::address::from_string("127.0.0.1", error);

  ASSERT_EQ(error, boost::system::error_code());
  ASSERT_EQ(client1.Connect(boost::asio::ip::tcp::endpoint(ip, port)), boost::system::error_code());
  ASSERT_EQ(client2.Connect(boost::asio::ip::tcp::endpoint(ip, port)), boost::system::error_code());
  ASSERT_EQ(client3.Connect(boost::asio::ip::tcp::endpoint(ip, port)), boost::system::error_code());

  const auto write_message = [&](dataapi::TcpClient& client, const std::string& message) {
    boost::asio::streambuf buffer;
    std::ostream stream{ &buffer };
    stream << message;
    client.Write(buffer);
  };

  const auto read_message = [&](dataapi::TcpClient& client, size_t size) -> std::string {
    boost::asio::streambuf buffer;
    client.Read(buffer, size);
    return boost::asio::buffer_cast<const char*>(buffer.data());
  };

  auto original = "Echo message from client1"s;
  write_message(client1, original);
  auto echo = read_message(client1, original.size());
  EXPECT_EQ(original, echo);

  original = "Another one echo message from client1"s;
  write_message(client1, original);
  echo = read_message(client1, original.size());
  EXPECT_EQ(original, echo);

  original = "Echo message from client2"s;
  write_message(client2, original);
  echo = read_message(client2, original.size());
  EXPECT_EQ(original, echo);

  original = "Another one echo message from client2"s;
  write_message(client2, original);
  echo = read_message(client2, original.size());
  EXPECT_EQ(original, echo);

  original = "Echo message from client3"s;
  write_message(client3, original);
  echo = read_message(client3, original.size());
  EXPECT_EQ(original, echo);

  original = "Another one echo message from client3"s;
  write_message(client3, original);
  echo = read_message(client3, original.size());
  EXPECT_EQ(original, echo);

  server.Stop();
}

TEST(UnitTests, BoostClientServerEchoInterruption) {
  unsigned short port = 44444;
  dataapi::TcpClient client;
  dataapi::EchoTcpServer server{ port };
  server.Start();

  boost::system::error_code error;
  const auto ip = boost::asio::ip::address::from_string("127.0.0.1", error);

  ASSERT_EQ(error, boost::system::error_code());
  ASSERT_EQ(client.Connect(boost::asio::ip::tcp::endpoint(ip, port)), boost::system::error_code());

  const auto write_message = [&](dataapi::TcpClient& client, const std::string& message) {
    boost::asio::streambuf buffer;
    std::ostream stream{ &buffer };
    stream << message;
    return client.Write(buffer);
  };

  const auto read_message = [&](dataapi::TcpClient& client, size_t size) -> std::string {
    boost::asio::streambuf buffer;
    client.Read(buffer, size);
    return boost::asio::buffer_cast<const char*>(buffer.data());
  };

  auto original = "Echo message"s;
  error = write_message(client, original);
  ASSERT_EQ(error, boost::system::error_code());
  auto echo = read_message(client, original.size());
  EXPECT_EQ(original, echo);

  original = "Another one echo message"s;
  error = write_message(client, original);
  ASSERT_EQ(error, boost::system::error_code());
  echo = read_message(client, original.size());
  EXPECT_EQ(original, echo);

  server.Stop();

  original = "Third echo message"s;
  error = write_message(client, original);
  EXPECT_EQ(error, boost::asio::error::eof);
}


}
