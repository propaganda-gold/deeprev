#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "api/router.h"
#include "data/connection.h"
#include "data/globals.h"
#include "data/vectorbook.pb.h"
#include "handlers/api.h"
#include "handlers/delegating_handler.h"
#include "handlers/requests.h"
#include "network/init.h"
#include "server/http_server.h"
#include "server/options.h"
#include "server/server_https.hpp"
#include "util/includes.h"
#include "util/mongo_adapter.h"
#include <boost/filesystem.hpp>
#include <memory>

ABSL_FLAG(std::string, rest_options, "", "");
ABSL_FLAG(std::string, secrets, "", "Path to a pbtxt of type 'Secrets'.");
ABSL_FLAG(std::string, model_spec, "", "");
ABSL_FLAG(int, port, 8090, "The port number for the server.");

using SimpleHttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using namespace std;
using namespace vectorbook;

Void Main() {
  auto rest_options = absl::GetFlag(FLAGS_rest_options);
  ASSERT(!rest_options.empty());
  auto secrets = absl::GetFlag(FLAGS_secrets);
  ASSERT(!secrets.empty());

	ASSERT_SUCCEEDS(curl::Initialize());
  ASSERT_SUCCEEDS(statics::LoadRestOptions(rest_options));
  ASSERT_SUCCEEDS(statics::LoadSecrets(secrets));
	ASSERT_SUCCEEDS(CreateRedisConnection());

	ASSERT_SUCCEEDS(mongodb::Initialize(statics::secrets().mongodb_uri()));


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
