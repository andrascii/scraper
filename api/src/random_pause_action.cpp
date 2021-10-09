#include "random_pause_action.h"

namespace api {

RandomPauseAction::RandomPauseAction(const nlohmann::json& json)
  : OptionalAction{json} {
  from_ = Ms{json.at("fromMs").get<uint64_t>()};
  to_ = Ms{json.at("toMs").get<uint64_t>()};

  if (from_ < to_) {
    throw std::invalid_argument{"'from' must be less than 'to' or equal"};
  }
}

RandomPauseAction::RandomPauseAction(
  bool optional,
  const Ms& from,
  const Ms& to
) : OptionalAction{optional},
    from_{from},
    to_{to} {
  if (from_ < to_) {
    throw std::invalid_argument{"'from' must be less than 'to' or equal"};
  }
}

nlohmann::json RandomPauseAction::Serialize() const {
  nlohmann::json json;
  json["fromMs"] = from_.count();
  json["toMs"] = to_.count();
  json.update(OptionalAction::Serialize());
  return json;
}

const RandomPauseAction::Ms& RandomPauseAction::FromMs() const noexcept {
  return from_;
}

const RandomPauseAction::Ms& RandomPauseAction::ToMs() const noexcept {
  return to_;
}

void RandomPauseAction::Accept(IActionVisitor* visitor) const {
  visitor->Visit(const_cast<RandomPauseAction*>(this));
}

}