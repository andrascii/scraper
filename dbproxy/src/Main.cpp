#include "DbProxyApplication.h"
#include "DbProxySettings.h"

namespace {

void QtMsgHandler(QtMsgType type, const QMessageLogContext&, const QString& msg) {
  switch (type) {
    case QtDebugMsg: {
      common::Logger()->debug("{:s}", msg.toStdString());
      break;
    }
    case QtInfoMsg: {
      common::Logger()->info("{:s}", msg.toStdString());
      break;
    }
    case QtWarningMsg: {
      common::Logger()->warn("{:s}", msg.toStdString());
      break;
    }
    case QtCriticalMsg: {
      common::Logger()->error("{:s}", msg.toStdString());
      break;
    }
    case QtFatalMsg: {
      common::Logger()->critical("{:s}", msg.toStdString());
      break;
    }
  }
}

}

int main(int argc, char** argv) {
  try {
    qInstallMessageHandler(QtMsgHandler);

    if (common::EnableConsoleLogging()) {
      std::cerr << "Enabling console logging error" << std::endl;
      return EXIT_FAILURE;
    }

    const auto expected_settings = api::DbProxySettings::read(argc, argv, "DbProxy", "DbProxy app");

    if (!expected_settings) {
      SPDLOG_CRITICAL("{:s}", expected_settings.error().message());
      return EXIT_FAILURE;
    }

    if (!expected_settings->enableConsoleLogging() && common::DisableConsoleLogging()) {
      std::cerr << "Error disabling console logging error" << std::endl;
      return EXIT_FAILURE;
    }

    common::Logger()->set_level(expected_settings->logLevel());
    api::DbProxyApplication app(argc, argv, *expected_settings);

    return app.exec();
  } catch (const std::exception& ex) {
    SPDLOG_CRITICAL("Application critical error: {:s}", ex.what());
    return EXIT_FAILURE;
  }
}

