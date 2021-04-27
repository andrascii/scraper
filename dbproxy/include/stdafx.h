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

//
// cxxopts
//
#include <cxxopts.hpp>

//
// Qt
//
#include <QCoreApplication>
#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaType>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QAuthenticator>
#include <QTimer>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMutex>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QSemaphore>
#include <QWaitCondition>
#include <QQueue>
#include <QDebug>
#include <QProcess>
#include <QDateTime>
#include <QAbstractNetworkCache>
#include <QStringBuilder>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QBuffer>
#include <QPointer>
#include <QSharedMemory>
#include <QDir>
#include <QUrlQuery>
#include <QJsonDocument>

using namespace std::chrono_literals;
using namespace std::string_literals;
