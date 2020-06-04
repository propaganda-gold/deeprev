#include "handlers/delegating_handler.h"

#include "absl/strings/str_split.h"
#include "handlers/api.h"
#include "util/includes.h"
#include <vector>
namespace vectorbook {

namespace impl {
class RouterHandler : public HttpHandler {
public:
  Option<HttpResponse> Handle(const HttpRequest &request) const override {
    Output_debug(request);
    for (auto &route_name : route_names_) {
      // HACK(greg) Tighten this up to check word boundaries.
      if (StartsWith(request.uri(), route_name)) {
        auto handler = route_handler_.const_get(route_name);
        ASSERT_EXISTS(handler);

        auto status = (*handler)->Handle(request);
        ASSERT_OK(status);
        return status;
      }
    }
    return error_handler_->Handle(request);
  }

public:
  NameHandlerMap route_handler_;
  vector<string> route_names_;
  Unique<HttpHandler> error_handler_;
};

} // namespace impl

namespace {
bool CompareLen(const std::string &a, const std::string &b) {
  return (a.size() < b.size());
}
} // namespace

Unique<HttpHandler> RouterHandler(NameHandlerMap &&route_handler,
                                  Unique<HttpHandler> &&error_handler) {
  auto result = new impl::RouterHandler();

  vector<string> routes;
  for (auto it : route_handler) {
    routes.push_back(it->key());
  }
  std::sort(routes.begin(), routes.end(), CompareLen);

  result->route_handler_ = std::move(route_handler);
  result->route_names_ = std::move(routes);
  result->error_handler_ = std::move(error_handler);
  return Unique<HttpHandler>(result);
}

} // namespace vectorbook
