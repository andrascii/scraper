#pragma once

namespace api {

class IMigrator {
 public:
  virtual ~IMigrator() = default;

  virtual void Apply() const = 0;
};

}