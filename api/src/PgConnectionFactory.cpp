#include "PgConnectionFactory.h"

namespace api {

SharedPgConnection PgConnectionFactory::Create(const SharedSettings& settings) {
  std::shared_ptr<PGconn> connection;

  connection.reset(PQsetdbLogin(
    settings->DatabaseHost().data(),
    std::to_string(settings->DatabasePort()).data(),
    nullptr,
    nullptr,
    settings->Database().data(),
    settings->DatabaseUser().data(),
    settings->DatabasePassword().data()
  ), &PQfinish);

  if (PQstatus(connection.get()) != ConnStatusType::CONNECTION_OK && PQsetnonblocking(connection.get(), 1) != 0) {
    throw std::runtime_error( PQerrorMessage( connection.get() ) );
  }

  return connection;
}

}