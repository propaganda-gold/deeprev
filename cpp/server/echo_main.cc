#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "handlers/delegating_handler.h"
#include "server/http_server.h"
#include "server/options.h"
#include "util/includes.h"
#include "util/smtp_mail.h"

using namespace vectorbook;

ABSL_FLAG(int, port, 8080, "The port number for the server.");

namespace {

class NotFoundHandler : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    HttpResponse response;
		response.set_code(404);
    response.set_content(
        Replace("<html><body>Not found URI: $uri", {{"$uri", request.uri()}}));
    return response;
  }
  static Unique<HttpHandler> Create() {
    return Unique<HttpHandler>(new NotFoundHandler());
  }
};

class EchoHandler : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
		Output_debug(request);
    auto result = MakeJson(request); ASSERT_OK(result);
		return result;
  }
  static Unique<HttpHandler> Create() {
    return Unique<HttpHandler>(new EchoHandler());
  }
};

Unique<HttpHandler> EchoMainHandler() {
  NameHandlerMap map = {};
  ASSERT_SUCCEEDS(map.put("/", EchoHandler::Create()));
  return RouterHandler(std::move(map), NotFoundHandler::Create());
}

} // namespace

Void Main() {
  HttpServerConfig config;
  config.address = "127.0.0.1";
  config.port = absl::GetFlag(FLAGS_port);

  auto handler = EchoMainHandler();
  ASSERT_OK(handler);
  auto server = CreateHttpServer(config, handler.get());
  ASSERT_OK(server);

  Output_() << "Starting server: http://" << config.address << ":"
            << config.port << "\n";
  auto start = server->Start();
  ASSERT_OK(start);

  return Ok();
}

#include "util/main_incl.h"
