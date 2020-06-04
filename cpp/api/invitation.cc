#include "api/invitation.h"

#include "data/globals.h"
#include "data/ops/api.pb.h"
#include "data/ops/emailtoken.h"
#include "data/ops/invitation.h"
#include "handlers/http.pb.h"
#include "util/random.h"

namespace vectorbook {

namespace {

Option<string> InvitationLink(const string &emailtoken) {
  string b;
  b += "http://localhost:8080/prod/invite/accept/";
  b += emailtoken;
  return b;
}

Void SendEmailInvitation(const string &email, const string &emailtoken) {
  auto link = InvitationLink(emailtoken);
  ASSERT_EXISTS(link);
  return Ok();
}

} // namespace
namespace impl {

class api_invitation_create : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto user_id = UserFromCookie(request);
    if (!user_id.has_value()) {
      return responses::LoginFailed();
    }

    auto email = ArgumentFromPost(request, "email");
    if (!email.has_value()) {
      return ClassError("POST must contain 'email'", "");
    }

    auto email_exists = EmailAlreadyRegistered(*email);
    ASSERT_OK(email_exists);

    if (*email_exists) {
      RestResponse response;
      response.set_success(false);
      response.mutable_error()->set_email_already_registered(true);
      return MakeJson(response);
    }

    auto status = CreateInvitation(*user_id, *email);
    ASSERT_OK(status);

    auto emailtoken = CreateEmailToken(*email);
    ASSERT_EXISTS(emailtoken);
    ASSERT_SUCCEEDS(SendEmailInvitation(*email, *emailtoken));
    return BasicSuccess();
  }
};

} // namespace impl

Unique<HttpHandler> api_invitation_create() {
  return Unique<HttpHandler>(new impl::api_invitation_create());
}

namespace impl {

class api_invitation_list : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto user_id = UserFromCookie(request);
    if (!user_id.has_value()) {
      return responses::LoginFailed();
    }

    auto invitations = InvitationListForUser(*user_id, "");
    ASSERT_OK(invitations);

    RestResponse response;
    response.set_success(true);
    *response.mutable_invitation_list() = *invitations;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_invitation_list() {
  return Unique<HttpHandler>(new impl::api_invitation_list());
}

namespace impl {

class api_invitation_delete : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    return Null();
  }
};

} // namespace impl

Unique<HttpHandler> api_invitation_delete() {
  return Unique<HttpHandler>(new impl::api_invitation_delete());
}

namespace impl {

class api_invitation_optout_add : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    return Null();
  }
};

} // namespace impl

Unique<HttpHandler> api_invitation_optout_add() {
  return Unique<HttpHandler>(new impl::api_invitation_optout_add());
}

} // namespace vectorbook
