#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "data/connection.h"
#include "data/globals.h"
#include "data/vectorbook.pb.h"
#include "network/curl_download.h"
#include "handlers/api.h"
#include "handlers/delegating_handler.h"
#include "handlers/requests.h"
#include "server/http_server.h"
#include "server/options.h"
#include "server/server_https.hpp"
#include "api/router.h"
#include "util/includes.h"
#include <boost/filesystem.hpp>
#include <memory>

ABSL_FLAG(std::string, http_options, "", "");
ABSL_FLAG(std::string, model_spec, "", "");
ABSL_FLAG(int, port, 8090, "The port number for the server.");

using SimpleHttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using namespace std;
using namespace vectorbook;

Void Main() {
  auto http_options = absl::GetFlag(FLAGS_http_options);
  ASSERT(!http_options.empty());
  ASSERT_SUCCEEDS(LoadRestOptions(http_options));
	ASSERT_SUCCEEDS(CreateRedisConnection());
	ASSERT_SUCCEEDS(curl::Initialize());

  HttpServerConfig config;
  config.address = "0.0.0.0";
  config.port = absl::GetFlag(FLAGS_port);

  auto handler = RestRouter();
  ASSERT_OK(handler);
  auto server = CreateHttpServer(config, handler.get());
  // ASSERT(server != nullptr);

  Output_() << "Starting server: http://" << config.address << ":"
            << config.port << "\n";
  auto start = server->Start();
  ASSERT_OK(start);

	ASSERT_SUCCEEDS(curl::CleanUp());
	return Ok();
}

#include "util/main_incl.h"
