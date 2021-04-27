#include "DbProxyApplication.h"
#include "Logger.h"
#include "Errors.h"

namespace api {

DbProxyApplication::DbProxyApplication(int argc, char** argv, const DbProxySettings& settings)
  : QCoreApplication{ argc, argv },
    settings_{ settings } {}

}
