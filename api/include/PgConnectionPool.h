#pragma once

#include "PgConnectionFactory.h"
#include "Settings.h"

namespace api {

class PgConnectionPool final : public std::enable_shared_from_this<PgConnectionPool> {
public:
  class ConnectionWrapper final {
  public:
    using Cleaner = std::function<void(const SharedPgConnection&)>;

    ConnectionWrapper(const SharedPgConnection& connection, Cleaner cleaner);
    ConnectionWrapper& operator=(ConnectionWrapper&& other) = delete;
    ConnectionWrapper& operator=(const ConnectionWrapper& other) = delete;
    ~ConnectionWrapper();

    const SharedPgConnection& Connection() const noexcept;

  private:
    Cleaner cleaner_;
    SharedPgConnection connection_;
  };

  PgConnectionPool(const SharedSettings& settings, size_t connection_count);

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