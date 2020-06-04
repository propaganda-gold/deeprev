#ifndef server_http_server_h
#define server_http_server_h
#include "server/connection.h"
#include "util/includes.h"
#include "handlers/api.h"

namespace vectorbook {

class HttpServerConfig {
public:
  /// Port number to use. Defaults to 80 for HTTP and 443 for HTTPS. Set to 0
  /// get an assigned port.
  unsigned short port;
  /// If io_service is not set, number of threads that the server will use
  /// when start() is called. Defaults to 1 thread.
  std::size_t thread_pool_size = 1;
  /// Timeout on request handling. Defaults to 5 seconds.
  long timeout_request = 5;
  /// Timeout on content handling. Defaults to 300 seconds.
  long timeout_content = 300;
  /// Maximum size of request stream buffer. Defaults to architecture maximum.
  /// Reaching this limit will result in a message_size error code.
  std::size_t max_request_streambuf_size =
      std::numeric_limits<std::size_t>::max();
  /// IPv4 address in dotted decimal form or IPv6 address in hexadecimal
  /// notation. If empty, the address will be any address.
  std::string address;
  /// Set to false to avoid binding the socket to an address that is already
  /// in use. Defaults to true.
  bool reuse_address = true;
};

class AbstractServer {
public:
  virtual Void Start() = 0;
};

// 'root_handler' must outlive result.
Unique<AbstractServer> CreateHttpServer(const HttpServerConfig &config,
                                        const HttpHandler *root_handler);

} // namespace vectorbook
#endif /* server_http_server_h */
