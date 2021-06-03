#pragma once

#include "pg_connection_factory.h"
#include "settings.h"

namespace api {

class PgConnectionPool final : public std::enable_shared_from_this<PgConnectionPool> {
 public:
  class ConnectionWrapper final {
   public:
    using Cleaner = std::function<void(const SharedPgConnection&)>;

    ConnectionWrapper(SharedPgConnection connection, Cleaner cleaner);
    ConnectionWrapper(ConnectionWrapper&& other) = default;
    ConnectionWrapper(const ConnectionWrapper& other) = delete;
    ConnectionWrapper& operator=(ConnectionWrapper&& other) = delete;
    ConnectionWrapper& operator=(const ConnectionWrapper& other) = delete;
    ~ConnectionWrapper();

    [[nodiscard]] const SharedPgConnection& Connection() const noexcept;

   private:
    Cleaner cleaner_;
    SharedPgConnection connection_;
  };

  PgConnectionPool(SharedSettings settings, size_t connection_count);
  PgConnectionPool(PgConnectionPool&& other) = delete;
  PgConnectionPool(const PgConnectionPool& other) = delete;
  PgConnectionPool& operator=(PgConnectionPool&& other) = delete;
  PgConnectionPool& operator=(const PgConnectionPool& other) = delete;
  ~PgConnectionPool() = default;

  ConnectionWrapper Take() noexcept;

 private:
  void Free(const SharedPgConnection& to_free) noexcept;

 private:
  struct Descriptor {
    SharedPgConnection connection;
    bool is_acquired = false;
  };

  std::mutex mutex_;
  std::condition_variable condition_;
  std::vector<Descriptor> pool_;
  SharedSettings settings_;
};

}