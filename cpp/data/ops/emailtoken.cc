#include "data/ops/emailtoken.h"

#include "network/send_sendgrid.h"
#include "data/globals.h"
#include "util/includes.h"
#include "util/secure_token.h"
#include "server/options.h"

namespace vectorbook {

Option<string> TryResolveEmailToken(const string &emailtoken) {
  // Check that 'emailtoken' is in the database.
  auto email = data::emailtoken_map()->const_get(emailtoken);
  ASSERT_OK(email);
  if (email.has_value()) {
    return *email;
  } else {
    return Null();
  }
}

Option<string> CreateEmailToken(const string &email) {
  auto emailtoken = FreshSecureToken();
  ASSERT_EXISTS(emailtoken);
  auto status = data::emailtoken_map()->put(*emailtoken, email);
  ASSERT_OK(status);
  return *emailtoken;
}

Void EraseEmailToken(const string &emailtoken) {
  auto status = data::emailtoken_map()->erase(emailtoken);
  ASSERT_OK(status);
  return Ok();
}

namespace {

string MakeAddress(const string& path, const string& token) {
  string buffer = statics::rest_options().frontend_domain();
	buffer += path;
	buffer += '/';
  buffer += token;
  return buffer;
}

Option<SendgridRequest> StartLoginRequest(const string& token) {
	SendgridRequest request;
	request.set_subject("Log-In to Propaganda Gold");
	request.set_body("Log-In to Propaganda Gold");
//	auto url = MakeAddress("/prod/login/cookietoken", token);
//	request.set_body(Replace(R"(
//	<html><body>
//		<a href="$url">$url</a>
//	</body></html>
//	)", {{"$url", url}}));
	return request;
}

Option<SendgridRequest> StartSignupRequest(const string& token) {
	SendgridRequest request;
	request.set_subject("Sign-Up for Propaganda Gold");
	// request.set_body("Sign-Up for Propaganda Gold");
	auto url = MakeAddress("/prod/user/create", token);
	string body = StrReplaceAll2(R"(<html><body><a href='$url'>$url</a></body></html>)", {{"$url", url}});
	Print_value(body);
	request.set_body(body);
	return request;
}

bool ActuallySendEmail(const string& email) {
	Print_value(email);
	Print_debug(statics::rest_options());
	Print_debug(statics::secrets());

	if (statics::rest_options().use_production_email()) {
		Print_line();
		// Note: Fastest condition for production.
		return true;
	} else if (statics::rest_options().use_whitelist_email()) {
		Print_line();
		// Note: Only for development.
		for (const string& whitelist_email : statics::secrets().whitelisted_email()) {
			if (email == whitelist_email) return true;
		}
		return false;
	} else {
		Print_line();
		return false;
	}
}

} // namespace


// Assumes that 'email' is validated.
Void CreateTokenAndSendEmail(const string& email) {
	auto user_id = data::email_map()->const_get(email);
	ASSERT_OK(user_id);

	auto token = CreateEmailToken(email);
	ASSERT_EXISTS(token);

	Option<SendgridRequest> send_request;
	if (user_id.has_value()) {
		send_request = StartLoginRequest(*token);
		ASSERT_EXISTS(send_request);
	} else {
		send_request = StartSignupRequest(*token);
		ASSERT_EXISTS(send_request);
	}
	send_request->set_to_address(email);
	send_request->set_from_address("mail@propaganda.gold");
  send_request->set_from_name("c++ tester");
	send_request->set_secret(statics::secrets().sendgrid_api());

	bool actually_send = ActuallySendEmail(email);
	Print_value(actually_send);
	Output_debug(*send_request);
	if (actually_send) {
		ASSERT_SUCCEEDS(SendSendgrid(*send_request));
	}
	return Ok();
}

} // namespace vectorbook
