#include "api/broadcast.h"

// include start
#include "data/globals.h"
#include "data/converter.h"
#include "util/cassandra_logging.h"
#include "data/ops/broadcast.h"
#include "handlers/http.pb.h"

namespace vectorbook {

namespace impl {

class api_broadcast_create : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    Broadcast::BasicData basic_data;
    auto user_id = UserFromCookie(request);
    ASSERT_EXISTS(user_id);
    basic_data.set_producer(*user_id);

    auto textarea = ArgumentFromPost(request, "textarea");
    ASSERT_EXISTS(textarea);
    basic_data.set_textarea(*textarea);
    auto reference = ArgumentFromPost(request, "reference");
    ASSERT_OK(reference);
		if (reference.has_value()) {
			basic_data.set_reference(*reference);
			auto polarity = ArgumentFromPost(request, "polarity");
			ASSERT_EXISTS(polarity);
			basic_data.set_polarity(*polarity);
		}

		Print_debug(basic_data);
    auto broadcast = BuildBroadcast(basic_data);
    ASSERT_OK(broadcast);

    RestResponse response;
    response.set_success(true);
    *response.mutable_broadcast() = *broadcast;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_broadcast_create() {
  return Unique<HttpHandler>(new impl::api_broadcast_create());
}

namespace impl {

class api_broadcast_read : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
		// Require user to login to see any broadcast.
    auto user_id = UserFromCookie(request);
    ASSERT_EXISTS(user_id);

    auto id = ArgumentFromPost(request, "id");
    ASSERT_EXISTS(id);
		Print_value(*id);

		auto pbtxt = GetFromCassandra("keyspace1.broadcast", *id);
    ASSERT_EXISTS(pbtxt);

    Broadcast result;
    ASSERT(TextFormat::ParseFromString(*pbtxt, &result));

//    auto broadcast = data::broadcast_map()->const_get(*id);
//    ASSERT_EXISTS(broadcast);

    RestResponse response;
    response.set_success(true);
    *response.mutable_broadcast() = result;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_broadcast_read() { return new impl::api_broadcast_read(); }

namespace impl {

class api_broadcast_delete : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto user_id = UserFromCookie(request);
    ASSERT_EXISTS(user_id);
    auto id = ArgumentFromPost(request, "id");
    ASSERT_EXISTS(id);

    // Condition 1: Broadcast actually exists.
    auto broadcast = data::broadcast_map()->const_get(*id);
    ASSERT_EXISTS(broadcast);

    // Condition 2: Producer is this user.
    ASSERT(broadcast->basic_data().producer() == *user_id);

    // Result: Erase the broadcast.
    ASSERT_SUCCEEDS(data::broadcast_map()->erase(*id));

    // Return.
    return BasicSuccess();
  }
};

} // namespace impl

Unique<HttpHandler> api_broadcast_delete() {
  return new impl::api_broadcast_delete();
}

} // namespace vectorbook
