#pragma once

namespace api {

struct HelpMessageRequested : std::runtime_error { using std::runtime_error::runtime_error; };
struct HttpServerInitializationError : std::runtime_error { using std::runtime_error::runtime_error; };

}
