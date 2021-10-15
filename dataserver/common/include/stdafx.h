#pragma once

//
// C/C++
//
#include <cstdlib>
#include <vector>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <atomic>
#include <memory>
#include <utility>
#include <cassert>
#include <functional>
#include <optional>
#include <csignal>
#include <optional>
#include <iostream>

//
// Boost
//
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>

//
// Spdlog
//
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#if defined(__GNUC__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if defined(__GNUC__)
  #pragma GCC diagnostic pop
#endif

//
// tl::expected
//
#include <tl/expected.hpp>

#if defined(_WIN32)
  #define USE_WINDOWS_SET_THREAD_NAME_HACK
  #include <windows.h>
#elif defined(__APPLE__)
  #include <pthread.h>
#else
  #include <sys/prctl.h>
#endif

using namespace std::chrono_literals;
using namespace std::string_literals;
