#include "data/connection.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "data/connection.h"
#include "data/redis.pb.h"
namespace vectorbook {
//using namespace std;
//using namespace absl;
//using google::protobuf::util::Void;
//using google::protobuf::util::Unique;
//TEST(ConnectionTest, Get) {
//	RedisConnection* connection = StaticRedisConnection();
//	auto status = connection->Connect("127.0.0.1", 6379);
//	ASSERT_TRUE(status.ok());
//	{
//		auto reply_or = connection->Execute({"SET %s %s", "hello", "giant"});
//		ASSERT_TRUE(reply_or.ok());
//	}
//	{
//		auto reply_or = connection->Execute({"GET %s", "hello"});
//		ASSERT_TRUE(reply_or.ok());
//	}
//
//	ASSERT_TRUE(connection->Disconnect().ok());
//	/*
//	auto connection_or = Connection::Connect("127.0.0.1", 6379);
//	auto& connection = connection_or.ValueOrDie();
//	RedisRequest request;
//	request.set_query("get hello");
//	auto status_or = connection.Execute(request);
//	auto s = status_or.ValueOrDie().DebugString();
//	connection.Disconnect();
//	*/
//}
///*
//TEST(ConnectionTest, Set) {
//	auto connection_or = Connection::Connect("127.0.0.1", 6379);
//	auto& connection = connection_or.ValueOrDie();
//	{
//		RedisRequest request;
//		request.set_query("set %s %s");
//		request.add_argument("foo");
//		request.add_argument("hello world");
//		auto status_or = connection.Execute(request);
//		auto s = status_or.ValueOrDie().DebugString();
//	}
//	{
//		RedisRequest request;
//		request.set_query("get %s");
//		request.add_argument("foo");
//		auto status_or = connection.Execute(request);
//		auto s = status_or.ValueOrDie().DebugString();
//	}
//	connection.Disconnect();
//}
//*/
} // namespace redis
