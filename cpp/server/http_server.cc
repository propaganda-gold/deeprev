// Source: https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
// The 5 Steps of a TCP
// Step 1: Create a TCP socket.
// Step 2: Bind the socket to the server address.
// Step 3: Listen
// Step 4: Accept
// Step 5: Send/Receive
//
// 1 using create(), Create TCP socket.
// 2 using bind(), Bind the socket to server address.
// 3 using listen(), put the server socket in a passive mode, where it waits for
// the client to approach the server to make a connection
// 4 using accept(), At
// this point, connection is established between client and server, and they are
// ready to transfer data.
// 5 Go back to Step 3.

#include "server/http_server.h"

#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <mutex>
#include <regex>
#include <sstream>
#include <thread>
#include <unordered_set>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

#include "absl/strings/numbers.h"
#include "handlers/api.h"
#include "handlers/delegating_handler.h"
#include "handlers/http.pb.h"
#include "handlers/requests.h"
#include "server/connection.h"
#include "util/includes.h"

namespace asio = boost::asio;
using error_code = boost::system::error_code;
namespace errc = boost::system::errc;
namespace make_error_code = boost::system::errc;

namespace vectorbook {

using HTTP = asio::ip::tcp::socket;

namespace impl {

class HttpServer : public AbstractServer {

  // Step 5: Send/Receive
public:
  Void Start() override {
    Bind();
    Listen();
    Accept();
    Run();
    return Ok();
  }

private:
  Void Bind() {
    ASSERT(config_.address.size() > 0);
    // Step 1: Create a TCP socket. ?
    asio::ip::tcp::endpoint endpoint = asio::ip::tcp::endpoint(
        asio::ip::address::from_string(config_.address), config_.port);
    // Step 1: Create a TCP socket. ?
    acceptor_->open(endpoint.protocol());
    acceptor_->set_option(
        asio::socket_base::reuse_address(config_.reuse_address));
    // Step 2: Bind the socket to the server address.
    acceptor_->bind(endpoint);
    return Ok(); // return acceptor->local_endpoint().port();
  }

  // QUESTION(greg) Can Run go before Accept, and just make these different
  // methods?
  Void Run() {
    io_service_->reset();
    io_service_->run();
    return Ok();
  }
  Void Listen() {
    acceptor_->listen();
    return Ok();
  }

  // Tail Recursively Calls:
  //   Accept
  Void Accept() {
    auto connection = CreateConnection(*io_service_);
    // Step 4: Accept
    acceptor_->async_accept(
        *connection->socket_,
        [this, connection](const error_code &error_code0) {
          auto lock = connection->handler_runner_->continue_lock();
          if (!lock) {
            return;
          }

          // Note:
          //	Recursively call Accept.
          if (error_code0 != asio::error::operation_aborted) {
            this->Accept();
          }

          // There was an error.
          if (error_code0 != 0) {
            // TODO(P1) Warn louder.
            Output_value(error_code0);
            return;
          }

          // Read from the socket.
          asio::ip::tcp::no_delay option(true);
          auto session = std::make_shared<Session>(
              config_.max_request_streambuf_size, connection);
          error_code error_code1;
          session->connection_->socket_->set_option(option, error_code1);
          this->Read(session);
        });
    return Ok();
  }

  Void RespondToRequest(const string &string_request,
                        const std::shared_ptr<Session> &session) {
		Output_value(string_request);
    const HttpRequest full_parsed = ConvertHttpRequest(string_request);

    // TODO(greg) only create this once!
    // auto root_handler = RegisterMainHandler();
    Option<HttpResponse> return_response = root_handler_->Handle(full_parsed);
    Output_value(return_response.status());
		if (!return_response.ok()) {
			Output_value(return_response.error().StackTrace());
		}

    if (!return_response.has_value()) {
      return_response = MakeErrorHttpResponse(return_response.error().StackTrace());
    }

    ASSERT(return_response.has_value());
    Option<string> string_response = ConvertHttpResponse(*return_response);
		ASSERT(string_response.has_value());
		Output_value(*string_response);

    {
      asio::streambuf streambuf;
      std::ostream write_ostream(&streambuf);
      if (string_response.has_value()) {
        write_ostream << *string_response;
      } else {
        write_ostream << "HTTP/1.1 400 Server "
                         "Error\r\nConnection: Closed\r\n";
      }

      asio::async_write(*session->connection_->socket_, streambuf,
                        [](const error_code &error_code0,
                           std::size_t /*bytes_transferred*/) {});
    }
    return Ok();
  }

  Void FinishReadingAndRespond(size_t bytes_needed, const string &text_prefix,
                               const std::shared_ptr<Session> &session) {
    asio::async_read(
        *session->connection_->socket_, session->request_->streambuf_,
        asio::transfer_exactly(bytes_needed),
        [this, session, text_prefix](const error_code &error_code0,
                                     std::size_t /*bytes_transferred*/) {
          std::string text_request2(
              // FIX
              std::istreambuf_iterator<char>(session->request_->content_),
              (std::istreambuf_iterator<char>()));

          string full_request = text_prefix + text_request2;

          auto status = RespondToRequest(full_request, session);
          if (!status.ok()) {
            Output_value(status.status());
          }
          return;
        });
    return Ok();
  }

  Void Read(const std::shared_ptr<Session> &session) {
    session->connection_->set_timeout(config_.timeout_request);
    asio::async_read_until(
        *session->connection_->socket_, session->request_->streambuf_,
        "\r\n\r\n",
        [this, session](const error_code &error_code0,
                        std::size_t bytes_transferred) {
          session->connection_->cancel_timeout();

          if (error_code0) {
            throw "Error not handled";
          }
          const std::size_t num_additional_bytes =
              session->request_->streambuf_.size() - bytes_transferred;

          const std::string text_request(
              // FIX
              std::istreambuf_iterator<char>(session->request_->content_),
              (std::istreambuf_iterator<char>()));

          HttpRequest short_request = ConvertHttpRequest(text_request);

          auto content_length = HeaderArgument(short_request, "Content-Length");
          assert(content_length.ok());

          int int_result;
          if (content_length.has_value()) {
            bool parse_result = absl::SimpleAtoi(*content_length, &int_result);
            assert(parse_result);
          } else {
            int_result = 0;
          }

          const int content_length_int = int_result;
          const int bytes_needed = content_length_int - num_additional_bytes;

          // if (content_length_int > num_additional_bytes) {
          if (bytes_needed > 0) {
            auto status =
                FinishReadingAndRespond(bytes_needed, text_request, session);
            assert(status.ok());
            return;
          } else {
            auto status = RespondToRequest(text_request, session);
            assert(status.ok());
            return;
          }
        });
    return Ok();
  }

private:
  template <typename... Args>
  std::shared_ptr<Connection> CreateConnection(Args &&... args) noexcept {
    auto connections = this->connections_;
    auto connections_mutex = this->connections_mutex_;
    auto connection = std::shared_ptr<Connection>(
        new Connection(handler_runner_, std::forward<Args>(args)...),
        [connections, connections_mutex](Connection *connection) {
          {
            std::unique_lock<std::mutex> lock(*connections_mutex);
            auto it = connections->find(connection);
            if (it != connections->end())
              connections->erase(it);
          }
          // delete connection;
        });
    {
      std::unique_lock<std::mutex> lock(*connections_mutex);
      connections->emplace(connection.get());
    }
    return connection;
  }

public:
  // Note: These aren't actually public to any other file.
  std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;
  std::vector<std::thread> threads_;
  std::shared_ptr<asio::io_service> io_service_;
  std::unique_ptr<int> on_error_;
  HttpServerConfig config_;
  std::shared_ptr<std::unordered_set<Connection *>> connections_;
  std::shared_ptr<std::mutex> connections_mutex_;
  std::shared_ptr<ScopeRunner> handler_runner_;
  const HttpHandler *root_handler_ = nullptr;
};
} // namespace impl

Unique<AbstractServer> CreateHttpServer(const HttpServerConfig &config,
                                        const HttpHandler *root_handler) {
  auto result = new impl::HttpServer();
  result->io_service_ = std::make_shared<asio::io_service>();
  result->acceptor_ = std::unique_ptr<asio::ip::tcp::acceptor>(
      new asio::ip::tcp::acceptor(*result->io_service_));
  result->config_ = config;
  result->connections_.reset(new std::unordered_set<Connection *>());
  result->connections_mutex_.reset(new std::mutex());
  result->handler_runner_.reset(new ScopeRunner());
  result->root_handler_ = root_handler;
  return result;
}

} // namespace vectorbook
