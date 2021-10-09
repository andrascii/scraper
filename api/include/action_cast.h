#pragma once

#include "iaction.h"
#include "cast_action_visitor.h"

namespace api {

template <typename T>
T action_cast(IAction* action) {
  if (!action) {
    return nullptr;
  }

  CastActionVisitor<T> visitor;
  action->Accept(&visitor);
  return visitor.Result();
}

}