#include "random_pause_action.h"

namespace core {

RandomPauseAction::RandomPauseAction(const nlohmann::json& json) : OptionalAction{json} {
  from_ = Ms{json.at("fromMs").get<uint64_t>()};
  to_ = Ms{json.at("toMs").get<uint64_t>()};

  if (from_ < to_) {
    throw std::invalid_argument{"'from' must be less than 'to' or equal"};
  }
}

RandomPauseAction::RandomPauseAction(bool optional, const Ms& from, const Ms& to)
    : OptionalAction{optional},
      from_{from},
      to_{to} {
  if (from_ < to_) {
    throw std::invalid_argument{"'from' must be less than 'to' or equal"};
  }
}

nlohmann::json RandomPauseAction::Serialize() const {
  nlohmann::json json;
  json["fromMs"] = FromMs().count();
  json["toMs"] = ToMs().count();
  json["type"] = ActionToString(Type::kRandomPause);
  json.update(OptionalAction::Serialize());
  return json;
}

const RandomPauseAction::Ms& RandomPauseAction::FromMs() const noexcept { return from_; }

const RandomPauseAction::Ms& RandomPauseAction::ToMs() const noexcept { return to_; }

}// namespace core
