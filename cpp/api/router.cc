#include "api/router.h"

#include "api/application.h"
#include "api/broadcast.h"
#include "api/cookietoken.h"
#include "api/emailtoken.h"
#include "api/feed.h"
#include "api/invitation.h"
#include "api/label.h"
#include "api/responses.pb.h"
#include "api/resource.h"
#include "api/user.h"

namespace vectorbook {

namespace {

class NotFoundHandler : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
		auto r = ErrorBuilder();
		r.mutable_error()->set_route_not_found(request.uri());
		return MakeJson(r);
  }
  static Unique<HttpHandler> Create() {
    return Unique<HttpHandler>(new NotFoundHandler());
  }
};

} // namespace

Unique<HttpHandler> RestRouter() {
  NameHandlerMap map = {};
	// emailtoken ==============================================
  ASSERT_SUCCEEDS(map.put("/emailtoken/create", api_emailtoken_create()));
  ASSERT_SUCCEEDS(map.put("/emailtoken/validate", api_emailtoken_validate()));
	// cookietoken ==============================================
  ASSERT_SUCCEEDS(map.put("/cookietoken/create", api_cookietoken_create()));
  ASSERT_SUCCEEDS(map.put("/cookietoken/validate", api_cookietoken_validate()));
  ASSERT_SUCCEEDS(map.put("/cookietoken/destroy", api_cookietoken_destroy()));
	// user ==============================================
  ASSERT_SUCCEEDS(map.put("/user/create", api_user_create()));
  ASSERT_SUCCEEDS(map.put("/user/history", api_user_history()));
  ASSERT_SUCCEEDS(map.put("/user/read", api_user_read()));
  ASSERT_SUCCEEDS(map.put("/user/destroy", api_user_destroy()));
	// broadcast ==============================================
  ASSERT_SUCCEEDS(map.put("/broadcast/create", api_broadcast_create()));
  ASSERT_SUCCEEDS(map.put("/broadcast/read", api_broadcast_read()));
  ASSERT_SUCCEEDS(map.put("/broadcast/delete", api_broadcast_delete()));
	// label ==============================================
  ASSERT_SUCCEEDS(map.put("/label", api_label()));
	// feed ==============================================
  ASSERT_SUCCEEDS(map.put("/feed/single", api_feed_single()));
	// resource
  ASSERT_SUCCEEDS(map.put("/resource/get", api_resource_get()));

  return RouterHandler(std::move(map), NotFoundHandler::Create());
}

} // namespace vectorbook
