#ifndef HANDLERS_DELEGATING_HANDLER
#define HANDLERS_DELEGATING_HANDLER
#include "handlers/api.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace vectorbook {
using std::string;
using std::unordered_map;

// Map from handler name ot handler.
using NameHandlerMap = StdUniqueMap<string, Unique<HttpHandler>>;

// Routing handler routes to other handlers based on the path. This is
// effecitvely a web framework.
Unique<HttpHandler> RouterHandler(NameHandlerMap &&name_handler,
                                  Unique<HttpHandler> &&error_handler);

} // namespace vectorbook
#endif /* HANDLERS_DELEGATING_HANDLER */
