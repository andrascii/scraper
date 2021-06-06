#include "pg_connection_factory.h"

namespace api {

std::shared_ptr<pqxx::connection> PgConnectionFactory::Create(const SharedSettings& settings) {
  std::stringstream connection_string;

  connection_string
    << "postgresql://"
    << settings->DatabaseUser() << ":"
    << settings->DatabasePassword() << "@"
    << settings->DatabaseHost() << ":"
    << settings->DatabasePort() << "/"
    << settings->Database();

  return std::make_shared<pqxx::connection>(connection_string.str());
}

}