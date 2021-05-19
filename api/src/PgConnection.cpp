#include "PgConnection.h"

namespace api {

PgConnection::PgConnection(SharedDbProxySettings settings) {
  connection_.reset(PQsetdbLogin(
    settings->DatabaseHost().data(),
    std::to_string(settings->DatabasePort()).data(),
    nullptr,
    nullptr,
    settings->Database().data(),
    settings->DatabaseUser().data(),
    settings->DatabasePassword().data()
  ), &PQfinish);
}

}