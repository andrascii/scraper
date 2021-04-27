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

//
// GTest
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//
// Boost
//
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/placeholders.hpp>

//
// Spdlog
//
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

//
// Expected
//
#include <expected.hpp>

using namespace std::chrono_literals;
using namespace std::string_literals;
