#include "api/feed.h"

// include start
#include "data/globals.h"
#include "data/set.h"
#include "data/ops/broadcast.h"
#include "handlers/http.pb.h"

namespace vectorbook {

namespace impl {

namespace {

Option<string> GetRandomBroadcast() {
	auto id =  data::RedisRandomSetElement("broadcast_set");
	ASSERT_OK(id);
	return id;
}

Option<string> GetNextBroadcast(const string& user_id) {
	auto broadcast_id = data::recommendation_queue(user_id)->pop();
	ASSERT_OK(broadcast_id);

	if (broadcast_id.has_value()) {
		return *broadcast_id;
	}
	auto random_id = GetRandomBroadcast();
	ASSERT_OK(random_id);
	return random_id;
}

}

class api_feed_single : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto user_id = UserFromCookie(request);
    if (!user_id.has_value()) {
			return responses::LoginFailed();
		}

		auto broadcast_id = GetNextBroadcast(*user_id);
		ASSERT_OK(broadcast_id);

		auto broadcast_map = data::broadcast_map();

		if (!broadcast_id.has_value() || broadcast_id->empty()) {
			// No more broadcasts left.
			RestResponse response;
			response.set_success(false);
			response.mutable_error()->set_queue_empty(true);
			return MakeJson(response);
		}

		auto broadcast = broadcast_map->const_get(*broadcast_id);
		ASSERT_EXISTS(broadcast);

		RestResponse response;
		response.set_success(true);
		*response.mutable_broadcast() = *broadcast;
		return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_feed_single() {
  return Unique<HttpHandler>(new impl::api_feed_single());
}

} // namespace vectorbook
