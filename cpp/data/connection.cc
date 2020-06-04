#include "data/connection.h"
#include <stdlib.h>
#include <string.h>

#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/types/optional.h"
#include "hiredis.h"
#include "server/options.h"
#include <google/protobuf/stubs/status_macros.h>
#include <google/protobuf/stubs/statusor.h>

namespace vectorbook {

namespace {

RedisResponse ConvertResponse(const redisReply &native) {
  RedisResponse result;
  result.set_error_code(native.type);
  switch (native.type) {
  case 1:
    // String.
    result.set_string_value(native.str);
    break;
  case 2:
    // Array.
    for (size_t i = 0; i < native.elements; ++i) {
      redisReply *rx = native.element[i];
      *(result.add_child()) = ConvertResponse(*rx);
    }
    break;
  case 3:
    // Integer.
    result.set_int_value(native.integer);
    break;
  default:
    break;
  }
  return result;
}

// This is an actual data, for real use.
class RealRedisConnection : public RedisConnection {
public:
  // RealRedisConnection is created with no arguments. An init-time call must be
  // made to connect before.
  RealRedisConnection() {}

  // Connect should be called ONCE, during init-time.
  Void Connect(const string &host_name, int port) {
    const struct timeval timeout = {1, 500000}; // 1.5 seconds
    context_ = nullptr;
    context_ = redisConnectWithTimeout(host_name.c_str(), port, timeout);
    if (context_ == nullptr) {
      RAISE_EXCEPTION("RealRedisConnection is null.");
    } else if (context_->err) {
      redisFree(context_);
      RAISE_EXCEPTION("RealRedisConnection is error.");
    } else {
      return Ok();
    }
  }

  // Do we even need disconnect?
  Void Disconnect() {
    redisFree(context_);
    return Ok();
  }
  // Executes 'query' against the underlying databse.
  Unique<RedisResponse> Execute(const vector<string> &query) override {
    redisReply *reply;
    if (query.size() == 1) {
      reply = redisCommand(context_, query[0].c_str());
    } else if (query.size() == 2) {
      reply = redisCommand(context_, query[0].c_str(), query[1].c_str());
    } else if (query.size() == 3) {
      reply = redisCommand(context_, query[0].c_str(), query[1].c_str(),
                           query[2].c_str());
    } else if (query.size() == 4) {
      reply = redisCommand(context_, query[0].c_str(), query[1].c_str(),
                           query[2].c_str(), query[3].c_str());
    } else {
      RAISE_EXCEPTION("This many arguments are not implemented.");
    }

    RedisResponse result = ConvertResponse(*reply);
    freeReplyObject(reply);
    return new RedisResponse(result);
  }

private:
  // POINTER: Freed in Disconnect() using legacy c-language command.
  redisContext *context_ = nullptr;
};

RedisConnection *CreateAndConnect(const string& host_name) {
  auto result = new RealRedisConnection();
  auto status = result->Connect(host_name, 6379);
	if (!status.ok()) {
		std::cout << "Printing error: \n";
		PrettyPrintError(status);
    exit(EXIT_FAILURE);
	}
  return result;
}

} // namespace

static RedisConnection *static_connection = nullptr; // CreateAndConnect();
Void CreateRedisConnection(const string& host_name) {
  static_connection = CreateAndConnect(host_name);
  if (static_connection == nullptr) {
    RAISE_EXCEPTION("Problem creating database.");
  }
  return Ok();
}

Void CreateRedisConnection() {
	string server_name = statics::rest_options().redis_address();
	if (server_name.empty()) {
		server_name = "127.0.0.1";
	}
	auto status = CreateRedisConnection(server_name);
	ASSERT_OK(status);
	return Ok();
}


RedisConnection *StaticRedisConnection() {
  if (static_connection == nullptr) {
              Output_msg("CreateRedisConnection() before starting.");
    exit(EXIT_FAILURE);
  }
  return static_connection;
}

namespace impl {

// Normal compare, except: if the right query_part is "*", it will match any
// left part.
bool QueryEq(const vector<string> &a, const vector<string> &b) {
  if (a.size() != b.size())
    return false;
  for (int i = 0; i < a.size(); ++i) {
    if (b[i] == "*")
      continue;
    if (a[i] != b[i])
      return false;
  }
  return true;
}

// class MockRedisConnection : public RedisConnection {
// public:
//	MockRedisConnection(const vector<MockRedisPair>& pairs) : pairs_(pairs)
//{}
//
//	// Executes 'query' against the underlying databse.
//	Unique<RedisResponse> Execute(const vector<string>& query) override {
//		for (const MockRedisPair& pair : pairs_) {
//			bool r = QueryEq(query, pair.query);
//			if (r) {
//				return pair.response;
//			}
//		}
//		RAISE_EXCEPTION(StrCat("Can't find cached item for: ", Join(query, ",
//")));
//	}
// private:
//	vector<MockRedisPair> pairs_;
//};
//
//}  // namespace impl
//
// void MockRedisConnection(const vector<MockRedisPair>& mock_pairs) {
//	static_connection = new impl::MockRedisConnection(mock_pairs);
} // namespace impl

} // namespace vectorbook
