#include "api/label.h"

// include start
#include "data/globals.h"
#include "data/ops/broadcast.h"
#include "handlers/http.pb.h"
#include "util/uuid.h"

namespace vectorbook {

namespace impl {

class label : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto user_id = UserFromCookie(request);
    ASSERT_EXISTS(user_id);
    auto broadcast = ArgumentFromPost(request, "broadcast");
    ASSERT_EXISTS(broadcast);
    auto name = ArgumentFromPost(request, "name");
    ASSERT_EXISTS(name);
    auto value = ArgumentFromPost(request, "value");
    ASSERT_EXISTS(value);

    BooleanLabel label;
    label.set_name(*name);
    int int_value;
    ASSERT(absl::SimpleAtoi(*value, &int_value));
    label.set_value(int_value);

    Action action;
    auto action_id = FreshTimeUUID();
    ASSERT_EXISTS(action_id);
    action.set_id(*action_id);
    auto features = action.mutable_broadcast_features();
    features->set_consumer(*user_id);
    features->set_broadcast(*broadcast);

    auto object = data::broadcast_map()->const_get(*broadcast);
    ASSERT_EXISTS(object);
    features->set_producer(object->basic_data().producer());
    *action.add_label() = label;

    auto action_status = data::action_map()->put(action.id(), action);
    ASSERT_OK(action_status);

    ASSERT_SUCCEEDS(data::unsorted_action_queue()->append(action.id()));

    if (*name == "obscene") {
      ASSERT_SUCCEEDS(data::unsorted_obscenity_queue()->append(action.id()));
    }

    RestResponse response;
    response.set_success(true);
    *response.mutable_action() = action;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_label() {
  return Unique<HttpHandler>(new impl::label());
}

} // namespace vectorbook
