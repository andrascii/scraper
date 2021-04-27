#pragma once

#include "IApplication.h"
#include "Helpers.h"
#include "DbProxySettings.h"

namespace api {

class DbProxyApplication : public QCoreApplication, public IApplication {
  Q_OBJECT

public:
  DbProxyApplication(int argc, char** argv, const DbProxySettings& settings);

private:
  DbProxySettings settings_;
};

}
