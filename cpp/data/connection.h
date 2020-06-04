#ifndef REDIS_CONNECTION
#define REDIS_CONNECTION 
#include "hiredis.h"
#include "data/redis.pb.h"
#include "util/includes.h"
namespace vectorbook {

// This is an abstract connection so that it can be mocked out for test.
class RedisConnection {
public:
	virtual ~RedisConnection() {}
	// Executes 'query' against the underlying databse.
	virtual Unique<RedisResponse> Execute(const vector<string>& query) = 0;
};

// We maintain a static pool of data connections, because:
//   1) It would have to be piped to virtually all coe.
//   2) All traffic goes through the connection anyway.
// Any necessary locking will happen INSIDE this method, clients need not lock
// again.
RedisConnection* StaticRedisConnection();

Void CreateRedisConnection(const string& host_name);

// Use the host name in "server options".
Void CreateRedisConnection();

//struct MockRedisPair {
//	vector<string> query;
//	RedisResponse response;
//};
//
//// Mock the redis data for test.
//void MockRedisConnection(const vector<MockRedisPair>& mock_pairs);

}  // namespace vectorbook
#endif /* REDIS_CONNECTION */
