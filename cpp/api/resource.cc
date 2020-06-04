#include "api/resource.h"

#include "api/responses.pb.h"
#include "data/globals.h"
#include "data/ops/resource.h"
#include "handlers/http.pb.h"
#include "util/random.h"

namespace vectorbook {
namespace impl {

class api_resource_get : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const {
    // Parse argument.
    auto cookietoken = ArgumentFromPost(request, "cookietoken");
    if (!cookietoken.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("cookietoken");
      return MakeJson(r);
    }
    auto url = ArgumentFromPost(request, "url");
    if (!url.has_value()) {
      auto r = ErrorBuilder();
      r.mutable_error()->set_missing_input(true);
      r.mutable_error()->set_missing_argument("url");
      return MakeJson(r);
    }

		auto resource = ResourceFromUrl(*url);
		ASSERT_EXISTS(resource);


    RestResponse response;
    response.set_success(true);
    *response.mutable_resource() = *resource;
    return MakeJson(response);
  }
};

} // namespace impl

Unique<HttpHandler> api_resource_get() {
  return Unique<HttpHandler>(new impl::api_resource_get());
}

} // namespace vectorbook
