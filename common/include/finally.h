#pragma once

namespace common {

class Finally final {
 public:
  using Callback = std::function<void() noexcept>;

  Finally(Callback callback) : callback_{callback} {
    if (!callback_) {
      throw std::invalid_argument{"Passed empty callback"};
    }
  }

  Finally(Finally&& other) = delete;
  Finally(const Finally& other) = delete;

  Finally& operator=(Finally&& other) = delete;
  Finally& operator=(const Finally& other) = delete;

  ~Finally() {
    callback_();
  }

 private:
  Callback callback_;
};

}