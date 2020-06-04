#include "api/emailtoken.h"

// include start
#include "api/responses.pb.h"
#include "data/connection.h"
#include "data/globals.h"
#include "data/ops/user.h"
#include "data/ops/emailtoken.h"
#include "handlers/http.pb.h"
#include "server/options.h"
#include "util/secure_token.h"
#include "util/uuid.h"

namespace vectorbook {
namespace impl {

// Cases:
//  1. The user is registered, send log-in email.
//  2. The user is not registered, send sign-up email.
class emailtoken_create : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto email = ArgumentFromPost(request, "email");
    ASSERT_OK(email);
    if (!email.has_value()) {
      return ClassError("POST must contain 'email'", "");
    }
		ASSERT_SUCCEEDS(CreateTokenAndSendEmail(*email));
		return BasicSuccess();
  }
};

} // namespace impl

Unique<HttpHandler> api_emailtoken_create() {
  return Unique<HttpHandler>(new impl::emailtoken_create());
}

namespace impl {

class emailtoken_validate : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto emailtoken = ArgumentFromPost(request, "emailtoken");
    ASSERT_OK(emailtoken);
    if (!emailtoken.has_value()) {
      return ClassError("POST must contain 'emailtoken'", "");
    }

    auto token_result = data::emailtoken_map()->const_get(*emailtoken);
    ASSERT_OK(token_result);
    if (!token_result.has_value()) {
      return ClassError("'emailtoken' could not be found in database",
                        *emailtoken);
    }

    return BasicSuccess();
  }
};

} // namespace impl

Unique<HttpHandler> api_emailtoken_validate() {
  return new impl::emailtoken_validate();
}

} // namespace vectorbook
