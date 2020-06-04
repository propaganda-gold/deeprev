#include "server/http_server.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(HttpServer, Bind) {
	HttpServerConfig config; 
	config.address = "127.0.0.1";
	config.port = 8090;
	auto server = CreateHttpServer(config);
	ASSERT_TRUE(server.ok());

	auto start = server->Start();
	ASSERT_TRUE(start.ok());
}

} // namespace vectorbook
