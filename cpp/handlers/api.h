// The interface classes for the vectorbook package.
#ifndef HANDLERS_API
#define HANDLERS_API 

#include "handlers/http.pb.h"
#include "util/includes.h"
#include "util/protos.h"
#include "data/vectorbook.pb.h"
#include "api/responses.pb.h"

namespace vectorbook {

// Handles an HttpRequest.
class HttpHandler {
public:
	virtual Option<HttpResponse> Handle(const HttpRequest& request) const = 0;
};

// Tests a binary predicate on a HttpRequest.
class HttpPredicate {
public:
	virtual Unique<bool> Test(const HttpRequest& request) const = 0;
};

// Generic server.
class HttpServer {
public:
	virtual Void Start() = 0;
	virtual Void Stop() = 0;
};

string UnescapeHttpString(const string& escaped);
string EscapeHttpString(const string& escaped);


Option<string> CookieArgument(const HttpRequest &request, const string& key);
Option<string> HeaderArgument(const HttpRequest &request, const string& key);

// Argument from broadcast.
Option<string> ArgumentFromPost(const HttpRequest &request, const string& key);
Option<string> ArgumentFromGet(const HttpRequest &request, const string& key);

template <typename T> 
Option<HttpResponse> MakeJson(const T& proto) {
	auto json = ProtoToJsonString(proto); ASSERT_OK(json);
  HttpResponse result;
  result.set_code(200);
  result.set_json(*json + "\n");
  return result;
}

Option<HttpResponse> MakeError(const string& content) ;
Option<HttpResponse> MakeErrorHttpResponse(const string& content) ;
Option<HttpResponse> ClassError(const string& error_class, const string& error_value);

Option<string> UserFromCookie(const HttpRequest &request);

Option<HttpResponse> BasicSuccess();

// Note: Must not fail.
RestResponse ErrorBuilder();

namespace responses {
Option<HttpResponse> LoginFailed() ;
}  // namespace responses

} // namespace vectorbook
#endif /* HANDLERS_API */
