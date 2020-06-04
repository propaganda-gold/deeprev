#include "server/http_read_util.h"

#include "util/includes.h"

namespace vectorbook {

asio::async_read_until(
    *session_->connection->socket, session_->request->streambuf, "\r\n\r\n",
    [this, session_](const error_code &error_code,
                     std::size_t bytes_transferred) {
      session->connection->cancel_timeout();
      auto lock = session->connection->handler_runner->continue_lock();
      if (!lock) {
        return;
      }
      session->request->header_read_time = std::chrono::system_clock::now();
      if ((!error_code || error_code == asio::error::not_found) &&
          session->request->streambuf.size() ==
              session->request->streambuf.max_size()) {
        auto response = std::shared_ptr<Response>(
            new Response(session, this->config.timeout_content));
        response->write(StatusCode::client_error_payload_too_large);
        response->send();
        if (this->on_error_) {
          // this->on_error_(session->request, make_error_code::make_error_code(
          // errc::message_size));
        }
        return;
      }
      if (!error_code) {
        std::size_t num_additional_bytes =
            session->request->streambuf.size() - bytes_transferred;

        if (!RequestMessage::parse(
                session->request->content, session->request->method,
                session->request->path, session->request->query_string,
                session->request->http_version, session->request->header,
                session->request->full_request)) {
          if (this->on_error_) {
            // this->on_error_( session->request,
            // make_error_code::make_error_code(errc::protocol_error));
          }
          return;
        }

        auto header_it = session->request->header.find("Content-Length");
        if (header_it != session->request->header.end()) {
          unsigned long long content_length = 0;
          try {
            content_length = stoull(header_it->second);
          } catch (const std::exception &) {
            if (this->on_error_) {
              // this->on_error_( session->request,
              // make_error_code::make_error_code(errc::protocol_error));
            }
            return;
          }
          if (content_length > num_additional_bytes) {
            session->connection->set_timeout(config.timeout_content);
            asio::async_read(
                *session->connection->socket, session->request->streambuf,
                asio::transfer_exactly(content_length - num_additional_bytes),
                [this, session](const error_code &error_code,
                                std::size_t /*bytes_transferred*/) {
                  session->connection->cancel_timeout();
                  auto lock =
                      session->connection->handler_runner->continue_lock();
                  if (!lock) {
                    return;
                  }
                  if (!error_code) {
                    if (session->request->streambuf.size() ==
                        session->request->streambuf.max_size()) {
                      auto response = std::shared_ptr<Response>(
                          new Response(session, this->config.timeout_content));
                      response->write(
                          StatusCode::client_error_payload_too_large);
                      response->send();
                      if (this->on_error_) {
                        // this->on_error_(session->request,
                        // make_error_code::make_error_code(
                        // errc::message_size));
                      }
                      return;
                    }
                    this->find_resource(session);
                  } else if (this->on_error_)
                  // this->on_error_(session->request, error_code);
                });
          } else {
            this->find_resource(session);
          }
        } else if ((header_it =
                        session->request->header.find("Transfer-Encoding")) !=
                       session->request->header.end() &&
                   header_it->second == "chunked") {
          auto chunks_streambuf = std::make_shared<asio::streambuf>(
              this->config.max_request_streambuf_size);
          this->read_chunked_transfer_encoded(session, chunks_streambuf);
        } else
          this->find_resource(session);
      } else if (this->on_error_)
      // this->on_error_(session->request, error_code);
    });

} // namespace vectorbook
