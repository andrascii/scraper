#pragma once

namespace common {

template <typename T>
class NotNull {
 public:
  NotNull(std::nullptr_t) = delete;
  NotNull(T* pointer);

 private:
  T* pointer_;
};

template <> NotNull<std::nullptr_t>;

}
