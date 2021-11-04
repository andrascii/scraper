#pragma once

namespace common {

template <typename... Args>
struct VariantVisitor : Args... {
  using Args::operator()...;
};

template <typename... Args>
VariantVisitor(Args...) -> VariantVisitor<Args...>;

}// namespace common
