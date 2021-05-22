#include "PgConnectionPool.h"
#include "PgConnectionFactory.h"

namespace api {

PgConnectionPool::ConnectionWrapper::ConnectionWrapper(const SharedPgConnection& connection, Cleaner cleaner)
  : cleaner_{cleaner},
    connection_{connection} {}

PgConnectionPool::ConnectionWrapper::~ConnectionWrapper() {
  cleaner_(connection_);
}

const SharedPgConnection& PgConnectionPool::ConnectionWrapper::Connection() const noexcept {
  return connection_;
}


PgConnectionPool::PgConnectionPool(const SharedSettings& settings, size_t connection_count)
  : settings_{settings} {
  for (auto i{ 0u }; i < connection_count; ++i) {
    pool_.push_back(Descriptor{ PgConnectionFactory::Create(settings_) });
  }
}

PgConnectionPool::ConnectionWrapper PgConnectionPool::Take() noexcept {
  std::unique_lock lk{mutex_};

  const auto predicate = [](const Descriptor& connection) {
    return !connection.is_acquired;
  };

  condition_.wait(lk, [&] {
    return std::find_if(begin(pool_), end(pool_), predicate) != end(pool_);
  });

  const auto iterator = std::find_if(begin(pool_), end(pool_), predicate);
  assert(iterator != end(pool_) && "Iterator always must be valid!");

  iterator->is_acquired = true;

  return ConnectionWrapper{
    iterator->connection,
    [self = shared_from_this()](const SharedPgConnection& connection) {
      self->Free(connection);
    }
  };
}

void PgConnectionPool::Free(const SharedPgConnection& to_free) noexcept {
  std::lock_guard lk{mutex_};

  const auto predicate = [&](const Descriptor& connection) {
    return !connection.is_acquired;
  };

  const auto iterator = std::find_if(begin(pool_), end(pool_), predicate);

  if (iterator == end(pool_)) {
    return;
  }

  // if someone occasionally broke the taken connection
  // then we recreate it in the pool to ensure poll contains "connection_count" connections
  if (PQstatus(iterator->connection.get()) != ConnStatusType::CONNECTION_OK) {
    *iterator = Descriptor{ PgConnectionFactory::Create(settings_) };
  } else {
    iterator->is_acquired = false;
  }

  condition_.notify_all();
}

}