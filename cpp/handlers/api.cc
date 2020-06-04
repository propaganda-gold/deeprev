#include <unordered_map>

#include "data/globals.h"
#include "handlers/api.h"
#include "util/col.h"
#include "util/strings.h"

namespace vectorbook {

using SimpleStringMap = std::unordered_map<string, string>;

namespace {

SimpleStringMap *new_escape_map() {
  auto mapping = new SimpleStringMap();
  (*mapping)[" "] = "%20";
  (*mapping)["@"] = "%40";
  return mapping;
}

SimpleStringMap *new_unescape_map() {
  auto mapping = new SimpleStringMap();
  (*mapping)["%20"] = " ";
  (*mapping)["%40"] = "@";
  (*mapping)["%3A"] = ":";
  (*mapping)["%2F"] = "/";
  (*mapping)["%3F"] = "?";
  (*mapping)["%3D"] = "=";
  return mapping;
}

const SimpleStringMap &static_escape_map() {
  static SimpleStringMap *single = new_escape_map();
  return *single;
}

const SimpleStringMap &static_unescape_map() {
  static SimpleStringMap *single = new_unescape_map();
  return *single;
}

} // namespace

string UnescapeHttpString(const string &escaped) {
  return Replace(escaped, static_unescape_map());
}

string EscapeHttpString(const string &escaped) {
  return Replace(escaped, static_escape_map());
}

Option<string> ArgumentFromGet(const HttpRequest &request, const string &key) {
  auto raw = ProtoGet(request.get(), key);
  ASSERT_OK(raw);
  return raw;
}

Option<string> CookieArgument(const HttpRequest &request, const string &key) {
  return ProtoGet(request.cookies(), key);
}

Option<string> HeaderArgument(const HttpRequest &request, const string &key) {
  return ProtoGet(request.headers(), key);
}

Option<string> ArgumentFromPost(const HttpRequest &request, const string &key) {
  auto raw = ProtoGet(request.post(), key);
  ASSERT_OK(raw);
	if (raw.has_value()) {
		return UnescapeHttpString(*raw);
	} else {
		return raw;
	}
}

Option<HttpResponse> MakeError(const string &content) {
  ErrorResponse e;
  e.set_error(content);

  HttpResponse result;
  result.set_code(400);
  auto json = ProtoToJsonString(e);
  ASSERT_OK(json);
  result.set_json(*json + "\n");
  return result;
}

Option<HttpResponse> MakeErrorHttpResponse(const string &content) {
  return MakeError(content);
}

Option<HttpResponse> ClassError(const string &error_class,
                                const string &error_value) {
  RestResponse response;
  response.set_success(false);
  return MakeJson(response);
}

Option<string> UserFromCookie(const HttpRequest &request) {
  auto cookietoken = ArgumentFromPost(request, "cookietoken");
	Print_value(cookietoken.status());
  if (!cookietoken.has_value()) {
    return Null();
  }
	Print_value(*cookietoken);
  auto user_id = data::cookietoken_map()->const_get(*cookietoken);
	Print_value(user_id.status());
  ASSERT_EXISTS(user_id);

	Print_value(*cookietoken);
	Print_value(*user_id);

  return *user_id;
}

Option<HttpResponse> BasicSuccess() {
  RestResponse response;
  response.set_success(true);
  return MakeJson(response);
}

namespace responses {

Option<HttpResponse> LoginFailed() {
	RestResponse result;
	result.set_success(false);
	result.mutable_error()->set_login_failed(true);
	return MakeJson(result);
}

}  // namespace responses


RestResponse ErrorBuilder() {
	RestResponse r;
	r.set_success(false);
	return r;
}


} // namespace vectorbook
