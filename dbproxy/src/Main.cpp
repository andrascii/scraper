#include "DbProxyApplication.h"
#include "Settings.h"
#include "Exceptions.h"
#include "KafkaPublisher.h"

struct Data {
  int x;
  int y;
  int z;
};

int main(int argc, char** argv) {
  try {
    if (common::EnableConsoleLogging()) {
      std::cerr << "Enabling console logging error" << std::endl;
      return EXIT_FAILURE;
    }

    const auto expected_settings = api::Settings::Read(
      argc,
      argv,
      "DbProxy",
      "DbProxy app"
    );

    if (!expected_settings) {
      SPDLOG_CRITICAL("{:s}", expected_settings.error().message());
      return EXIT_FAILURE;
    }

    const auto& settings = *expected_settings;

    if (!settings->EnableConsoleLogging() && common::DisableConsoleLogging()) {
      std::cerr << "Error disabling console logging error" << std::endl;
      return EXIT_FAILURE;
    }

    common::Logger()->set_level(settings->LogLevel());
    api::DbProxyApplication app{settings, std::make_unique<api::KafkaPublisher>(settings)};
    const auto error = app.Start();

    if (error) {
      SPDLOG_CRITICAL("Error: {:s}", error.message());
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  } catch (const api::HelpMessageRequested& ex) {
    std::cout << ex.what() << std::endl;
  } catch (const std::exception& ex) {
    SPDLOG_CRITICAL("Application critical error: {:s}", ex.what());
    return EXIT_FAILURE;
  }
}

