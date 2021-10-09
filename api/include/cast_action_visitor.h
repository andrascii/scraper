#include "default_action_visitor.h"

namespace api {

template <typename T>
class CastActionVisitor;

template <>
class CastActionVisitor<T*> : public DefaultActionVisitor {
 public:
  CastActionVisitor() : result_{nullptr} {}

  T Result() const noexcept {
    return result_;
  }

  void Visit(T action) override {
    result_ = action;
  }

 private:
  T result_;
};

}