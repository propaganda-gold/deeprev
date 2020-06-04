#include "api/user.h"

// include start
#include "api/responses.pb.h"
#include "data/connection.h"
#include "data/globals.h"
#include "data/ops/cookietoken.h"
#include "data/ops/emailtoken.h"
#include "data/ops/user.h"
#include "handlers/http.pb.h"
#include "util/random.h"

namespace vectorbook {
namespace impl {

class api_user_create : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    // Parse argument.
    auto emailtoken = ArgumentFromPost(request, "emailtoken");
    if (!emailtoken.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("emailtoken");
      return MakeJson(r);
    }
    auto user_name = ArgumentFromPost(request, "user_name");
    if (!user_name.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("user_name");
      return MakeJson(r);
    }

    // Check that we have a valid emailtoken.
    auto email = TryResolveEmailToken(*emailtoken);
    ASSERT_OK(email);
    if (!email.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_emailtoken_not_found(true);
      return MakeJson(r);
    }

    // Check to see if the user_name is taken.
    auto user_id = TryUserIdFromName(*user_name);
    ASSERT_OK(user_id);
    if (user_id.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_user_name_already_registered(true);
      return MakeJson(r);
    }

    // Assume:
    //  * emailtoken is found
    //  * user_name is free
    auto user = CreateUserOp(*email, *user_name);
    ASSERT_EXISTS(user);

    // Erase the email token.
    ASSERT_SUCCEEDS(EraseEmailToken(*emailtoken));

    // Make a cookietoken.
    auto cookietoken = CreateCookieToken(user->id());

    RestResponse response;
    response.set_success(true);
    response.set_cookietoken(*cookietoken);
    *response.mutable_self() = *user;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_user_create() {
  return Unique<HttpHandler>(new impl::api_user_create());
}

namespace impl {

class api_user_destroy : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto user_id = UserFromCookie(request);
		if (!user_id.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_login_failed(true);
      return MakeJson(r);
		}
    ASSERT_SUCCEEDS(data::user_map()->erase(*user_id));

    return BasicSuccess();
  }
};

} // namespace impl

Unique<HttpHandler> api_user_destroy() { return new impl::api_user_destroy(); }

namespace impl {

class api_user_read : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    // Require user to login to see any broadcast.
    auto self_id = UserFromCookie(request);
		if (!self_id.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_login_failed(true);
      return MakeJson(r);
		}

    auto user_name = ArgumentFromPost(request, "user_name");
    ASSERT_EXISTS(user_name);
    auto user_id = data::user_name_map()->const_get(*user_name);
    ASSERT_EXISTS(user_id);

    auto user = data::user_map()->const_get(*user_id);
    ASSERT_EXISTS(user);
    auto public_user = ReduceUserToPublic(*user);
    ASSERT_EXISTS(public_user);

    RestResponse response;
    response.set_success(true);
    *response.mutable_public_user() = *public_user;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_user_read() { return new impl::api_user_read(); }

namespace impl {

class user_history : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto self_id = UserFromCookie(request);
		if (!self_id.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_login_failed(true);
      return MakeJson(r);
		}

    auto user_name = ArgumentFromPost(request, "user_name");
    ASSERT_EXISTS(user_name);
    auto user_id = data::user_name_map()->const_get(*user_name);
    ASSERT_EXISTS(user_id);

    auto history = data::broadcast_history(*user_id);

		const int kMaxHistorySize = 10;
    vector<string> id_list;
    for (auto it : *history) {
      ASSERT_EXISTS(it);
      id_list.push_back(*it);
    }

		vector<string> random_id;
		if (id_list.size() > 0) {
			random_id.push_back(RandomFromVector(id_list));
		}

    RestResponse response;
    response.set_success(true);
    for (const string &broadcast_id : random_id) {
      auto broadcast = data::broadcast_map()->const_get(broadcast_id);
      ASSERT_OK(broadcast);
      if (broadcast.has_value()) {
        *response.mutable_broadcast_list()->Add() = *broadcast;
      }
    }
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_user_history() { return new impl::user_history(); }

namespace impl {
class debug_user_create : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    // Parse argument.
    auto user_name = ArgumentFromPost(request, "user_name");
    if (!user_name.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("user_name");
      return MakeJson(r);
    }
    auto email = ArgumentFromPost(request, "email");
    if (!email.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("email");
      return MakeJson(r);
    }

    // Check to see if the user_name is taken.
    auto user_id = TryUserIdFromName(*user_name);
    ASSERT_OK(user_id);
    if (user_id.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_user_name_already_registered(true);
      return MakeJson(r);
    }

    // Assume:
    //  * emailtoken is found
    //  * user_name is free
    auto user = CreateUserOp(*email, *user_name);
    ASSERT_EXISTS(user);

    // Make a cookietoken.
    auto cookietoken = CreateCookieToken(user->id());

    RestResponse response;
    response.set_success(true);
    response.set_cookietoken(*cookietoken);
    *response.mutable_self() = *user;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> debug_user_create() {
  return Unique<HttpHandler>(new impl::debug_user_create());
}
namespace impl {

class debug_user_self : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto cookietoken = ArgumentFromPost(request, "cookietoken");
    if (!cookietoken.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("cookietoken");
      return MakeJson(r);
    }

    auto user_id = TryResolveCookieToken(*cookietoken);
    if (!user_id.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_login_failed(true);
      return MakeJson(r);
    }

    auto user = data::user_map()->const_get(*user_id);
    ASSERT_EXISTS(user);

    RestResponse response;
    response.set_success(true);
    *response.mutable_self() = *user;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> debug_user_self() {
  return Unique<HttpHandler>(new impl::debug_user_self());
}

} // namespace vectorbook
