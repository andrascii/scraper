#pragma once

#include "pg_connection_factory.h"
#include "settings.h"

namespace core {

//
// PgConnectionPool is thread safe
//
class PgConnectionPool final : public std::enable_shared_from_this<PgConnectionPool> {
 public:

  class ConnectionWrapper final {
   public:

    using Cleaner = std::function<void(const std::shared_ptr<pqxx::connection>&)>;

    ConnectionWrapper(std::shared_ptr<pqxx::connection> connection, Cleaner cleaner);
    ConnectionWrapper(ConnectionWrapper&& other) = default;
    ConnectionWrapper(const ConnectionWrapper& other) = delete;
    ConnectionWrapper& operator=(ConnectionWrapper&& other) = delete;
    ConnectionWrapper& operator=(const ConnectionWrapper& other) = delete;
    ~ConnectionWrapper();

    [[nodiscard]] const std::shared_ptr<pqxx::connection>& Get() const noexcept;

   private:

    Cleaner cleaner_;
    std::shared_ptr<pqxx::connection> connection_;
  };

  PgConnectionPool(std::shared_ptr<Settings> settings, size_t connection_count);
  PgConnectionPool(PgConnectionPool&& other) = delete;
  PgConnectionPool(const PgConnectionPool& other) = delete;
  PgConnectionPool& operator=(PgConnectionPool&& other) = delete;
  PgConnectionPool& operator=(const PgConnectionPool& other) = delete;
  ~PgConnectionPool() = default;

  ConnectionWrapper Take() noexcept;

 private:

  void Free(const std::shared_ptr<pqxx::connection>& to_free) noexcept;

 private:

  struct Descriptor {
    std::shared_ptr<pqxx::connection> connection;
    bool is_acquired = false;
  };

  std::mutex mutex_;
  std::condition_variable condition_;
  std::vector<Descriptor> pool_;
  std::shared_ptr<Settings> settings_;
};

}// namespace core