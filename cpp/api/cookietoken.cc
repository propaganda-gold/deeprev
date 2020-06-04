#include "api/cookietoken.h"

// include start
#include "api/responses.pb.h"
#include "data/connection.h"
#include "data/globals.h"
#include "data/ops/user.h"
#include "handlers/http.pb.h"
#include "util/secure_token.h"
#include "util/uuid.h"

namespace vectorbook {
namespace impl {

class cookietoken_create : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto emailtoken = ArgumentFromPost(request, "emailtoken");
    ASSERT_OK(emailtoken);
    if (!emailtoken.has_value()) {
      return ClassError("POST must contain 'emailtoken'", "");
    }

    auto email = data::emailtoken_map()->const_get(*emailtoken);
    if (!email.has_value()) {
      return ClassError("'emailtoken' not recognized", *emailtoken);
    }

		auto user_id = data::email_map()->const_get(*email);
		ASSERT_EXISTS(user_id);

    auto cookietoken = FreshSecureToken();
    ASSERT_EXISTS(cookietoken);
    ASSERT_SUCCEEDS(data::cookietoken_map()->put(*cookietoken, *user_id));
    ASSERT_SUCCEEDS(data::emailtoken_map()->erase(*emailtoken));

    RestResponse response;
    response.set_success(true);
    response.set_cookietoken(*cookietoken);
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_cookietoken_create() {
  return Unique<HttpHandler>(new impl::cookietoken_create());
}

namespace impl {

class cookietoken_validate : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto cookietoken = ArgumentFromPost(request, "cookietoken");
    ASSERT_OK(cookietoken);
    if (!cookietoken.has_value()) {
      return ClassError("POST must contain 'cookietoken'", "");
    }

    auto user_id = data::cookietoken_map()->const_get(*cookietoken);
    ASSERT_OK(user_id);
    if (!user_id.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_login_failed(true);
      return MakeJson(r);
    }

    auto user = data::user_map()->const_get(*user_id);
    ASSERT_OK(user);
    if (!user.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_login_failed(true);
      return MakeJson(r);
    }

    RestResponse response;
    response.set_success(true);
    *response.mutable_self() = *user;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_cookietoken_validate() {
  return new impl::cookietoken_validate();
}

namespace impl {

class cookietoken_destroy : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto cookietoken = ArgumentFromPost(request, "cookietoken");
    ASSERT_OK(cookietoken);
    if (!cookietoken.has_value()) {
      return ClassError("POST must contain 'cookietoken'", "");
    }

    ASSERT_SUCCEEDS(data::cookietoken_map()->erase(*cookietoken));

    return BasicSuccess();
  }
};

} // namespace impl

Unique<HttpHandler> api_cookietoken_destroy() {
  return new impl::cookietoken_destroy();
}

namespace impl {

class debug_cookietoken_create : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto user_name = ArgumentFromPost(request, "user_name");
    ASSERT_OK(user_name);
    if (!user_name.has_value()) {
      return ClassError("POST must contain 'user_name'", "");
    }

    auto user_id = data::user_name_map()->const_get(*user_name);
    ASSERT_EXISTS(user_id);

    auto cookietoken = FreshSecureToken();
    ASSERT_EXISTS(cookietoken);
    ASSERT_SUCCEEDS(data::cookietoken_map()->put(*cookietoken, *user_id));

    RestResponse response;
    response.set_success(true);
    response.set_cookietoken(*cookietoken);
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> debug_cookietoken_create() {
  return Unique<HttpHandler>(new impl::debug_cookietoken_create());
}

} // namespace vectorbook
