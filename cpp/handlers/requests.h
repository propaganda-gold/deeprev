#ifndef HANDLERS_MS_REQUEST
#define HANDLERS_MS_REQUEST
#include "util/includes.h"
#include "handlers/http.pb.h"
#include "handlers/api.h"

namespace vectorbook {

using std::vector;
using std::string;

// Converts a string-valued request to an internal object.
HttpRequest ConvertHttpRequest(const string &original);

Option<string> ConvertHttpResponse(const HttpResponse& response);

Option<HttpResponse> MakeResponse(int code, const string& content);

} // namespace vectorbook
#endif /* HANDLERS_MS_REQUEST */
