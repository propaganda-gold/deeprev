#ifndef server_connection_h
#define server_connection_h 
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <mutex>
#include <regex>
#include <sstream>
#include <thread>
#include <unordered_set>

#include "util/includes.h"
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/streambuf.hpp>

namespace vectorbook {

namespace asio = boost::asio;
using HTTP = asio::ip::tcp::socket;
using error_code = boost::system::error_code;
namespace errc = boost::system::errc;
namespace make_error_code = boost::system::errc;

class ScopeRunner {
	/// Scope count that is set to -1 if scopes are to be canceled
	std::atomic<long> count;

public:
	class SharedLock {
		friend class ScopeRunner;
		std::atomic<long> &count;
		SharedLock(std::atomic<long> &count) noexcept : count(count) {}
		SharedLock &operator=(const SharedLock &) = delete;
		SharedLock(const SharedLock &) = delete;

	public:
		~SharedLock() noexcept {
			count.fetch_sub(1);
		}
	};

	ScopeRunner() noexcept : count(0) {}

	/// Returns nullptr if scope should be exited, or a shared lock otherwise
	std::unique_ptr<SharedLock> continue_lock() noexcept {
		long expected = count;
		while(expected >= 0 && !count.compare_exchange_weak(expected, expected + 1)) {
		}

		if(expected < 0) {
			return nullptr;
		} else {
			return std::unique_ptr<SharedLock>(new SharedLock(count));
		}
	}

	/// Blocks until all shared locks are released, then prevents future shared locks
	void stop() noexcept {
		long expected = 0;
		while(!count.compare_exchange_weak(expected, -1)) {
			if(expected < 0) {
				return;
			}
			expected = 0;
		}
	}
};

class Content : public std::istream {
public:
	std::size_t size() noexcept { return streambuf.size(); }
	/// Convenience function to return std::string. The stream buffer is
	/// consumed.
	std::string string() noexcept {
		try {
			std::string str;
			auto size = streambuf.size();
			str.resize(size);
			read(&str[0], static_cast<std::streamsize>(size));
			return str;
		} catch (...) {
			return std::string();
		}
	}

	asio::streambuf &streambuf;
	Content(asio::streambuf &streambuf) noexcept
			: std::istream(&streambuf), streambuf(streambuf) {}
};

class Request {
public:
	Request(std::size_t max_request_streambuf_size,
					std::shared_ptr<asio::ip::tcp::endpoint> remote_endpoint) noexcept
			: streambuf_(max_request_streambuf_size), content_(streambuf_),
				remote_endpoint_(std::move(remote_endpoint)) {}
	asio::streambuf streambuf_;
	Content content_;
	std::shared_ptr<asio::ip::tcp::endpoint> remote_endpoint_;
};

class Connection : public std::enable_shared_from_this<Connection> {
public:
	template <typename... Args>
	Connection(std::shared_ptr<ScopeRunner> handler_runner,
						 Args &&... args) noexcept
			: handler_runner_(std::move(handler_runner)),
				socket_(new HTTP(std::forward<Args>(args)...)) {}
  std::shared_ptr<ScopeRunner> handler_runner_;
  std::unique_ptr<HTTP> socket_;
  std::mutex socket_close_mutex_;
  std::unique_ptr<asio::steady_timer> timer_;
  std::shared_ptr<asio::ip::tcp::endpoint> remote_endpoint_;

  Void Close() noexcept {
    error_code ec;
    std::unique_lock<std::mutex> lock(socket_close_mutex_);
    // The following operations seems to be needed to run sequentially.
    socket_->lowest_layer().shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    socket_->lowest_layer().close(ec);
    return Ok();
  }

	Void set_timeout(long s) {
		return Ok();
	}
	Void cancel_timeout() {
		return Ok();
	}
};

class Session {
public:
	Session(std::size_t max_request_streambuf_size,
					std::shared_ptr<Connection> connection) 
			: connection_(std::move(connection)) {
		if (!connection_->remote_endpoint_) {
			error_code ec;
			connection_->remote_endpoint_ =
					std::make_shared<asio::ip::tcp::endpoint>(
							connection_->socket_->lowest_layer().remote_endpoint(ec));
		}
		request_ = std::shared_ptr<Request>(new Request(
				max_request_streambuf_size, connection_->remote_endpoint_));
	}

	std::shared_ptr<Connection> connection_;
	std::shared_ptr<Request> request_;
};

} // namespace vectorbook
#endif /* server_connection_h */
