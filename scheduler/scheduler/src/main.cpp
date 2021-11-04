#include "db_proxy_application.h"
#include "exceptions.h"
#include "http_handler_registry.h"
#include "kafka_publisher.h"
#include "migration_factory.h"
#include "settings.h"

namespace {

using namespace core;

Application* app_ptr;

std::string SignalToString(int signal) {
  switch (signal) {
    case SIGTERM: {
      return "SIGTERM";
    }
    case SIGSEGV: {
      return "SIGSEGV";
    }
    case SIGINT: {
      return "SIGINT";
    }
    case SIGILL: {
      return "SIGILL";
    }
    case SIGABRT: {
      return "SIGABRT";
    }
    case SIGFPE: {
      return "SIGFPE";
    }
    default: {
      return std::to_string(signal);
    }
  }
}

void OnAboutInterruptProcess(int signal) {
  SPDLOG_INFO("Received {:s} signal", SignalToString(signal));

  if (!app_ptr) {
    SPDLOG_CRITICAL("Completing application by calling std::exit cause app_ptr was not set");
    exit(EXIT_SUCCESS);
  }

  app_ptr->Stop();

  SPDLOG_INFO("Application stopped");
}

}// namespace

int main(int argc, char** argv) {
  try {
    if (common::EnableConsoleLogging()) {
      std::cerr << "Enabling console logging error" << std::endl;
      return EXIT_FAILURE;
    }

    const auto expected_settings = core::Settings::Read(
      argc,
      argv,
      "DbProxy",
      "DbProxy app");

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

    core::Application app{
      settings,
      //std::make_unique<core::KafkaPublisher>(settings),
      std::make_shared<MigrationFactory>(),
      std::make_shared<HttpHandlerRegistry>()};

    app_ptr = &app;
    signal(SIGINT, OnAboutInterruptProcess);

    const auto error = app.Start();

    if (error) {
      SPDLOG_CRITICAL("Error: {:s}", error.message());
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  } catch (const core::HelpMessageRequested& ex) {
    std::cout << ex.what() << std::endl;
  } catch (const std::exception& ex) {
    SPDLOG_CRITICAL("Application critical error: {:s}", ex.what());
    return EXIT_FAILURE;
  }
}
