#include "api/application.h"

#include "api/responses.pb.h"
#include "data/ops/application.h"
#include "data/ops/applytoken.h"
#include "server/options.h"
#include "util/check_recaptcha.h"
#include "util/includes.h"
#include "util/smtp.pb.h"

namespace vectorbook {

namespace {

Void SendApplyEmail(const string& email, const string &applytoken) {
	// TODO(greg) Dont hard code this.
  auto url = StrCat("http://vectorbook.social:8080/prod/application/verify?applytoken=", applytoken);
  return Ok();
}

}  // namespace

namespace impl {

class api_application_create : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    // Parse argument.
    auto email = ArgumentFromPost(request, "email");
    if (!email.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("email");
      return MakeJson(r);
    }
    auto textarea = ArgumentFromPost(request, "textarea");
    if (!textarea.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("textarea");
      return MakeJson(r);
    }

		if (statics::rest_options().recaptcha_required_for().application()) {
			auto recaptcha_token = ArgumentFromPost(request, "recaptcha_token");
			if (!recaptcha_token.has_value()) {
				auto r = ErrorBuilder();
				r.mutable_error()->set_missing_input(true);
				r.mutable_error()->set_missing_argument("recaptcha_token");
				return MakeJson(r);
			}
			auto recaptcha_success = GetRecaptchaSuccess(*recaptcha_token);
			ASSERT_EXISTS(recaptcha_success);
			if (!*recaptcha_success) {
				auto r = ErrorBuilder();
				r.mutable_error()->set_recaptcha_failure(true);
				return MakeJson(r);
			}
		}

		auto application = CreateApplication(*email, *textarea);
		ASSERT_EXISTS(application);

		auto applytoken = CreateApplyToken(application->id());
		ASSERT_EXISTS(applytoken);

		// Send the email.
		ASSERT_SUCCEEDS(SendApplyEmail(*email, *applytoken));

    RestResponse response;
    response.set_success(true);
		*response.mutable_application() = *application;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_application_create() {
  return Unique<HttpHandler>(new impl::api_application_create());
}

namespace impl {

class api_application_verify : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    auto applytoken = ArgumentFromPost(request, "applytoken");
    if (!applytoken.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("applytoken");
      return MakeJson(r);
    }

		auto application_id = TryResolveApplyToken(*applytoken) ;
		ASSERT_EXISTS(application_id);

		auto success = MarkApplicationVerified(*application_id);
		ASSERT_OK(success);

    RestResponse response;
    response.set_success(*success);
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_application_verify() {
  return Unique<HttpHandler>(new impl::api_application_verify());
}

} // namespace vectorbook
