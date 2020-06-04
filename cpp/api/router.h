#ifndef api_router_h
#define api_router_h 
#include "util/includes.h"
#include "handlers/api.h"
#include "handlers/delegating_handler.h"
#include "handlers/requests.h"

namespace vectorbook {

// Currently, RestRouter takes no options, and all configuration is hard-coded into
// the definition.
Unique<HttpHandler> RestRouter() ;

} // namespace vectorbook
#endif /* api_router_h */
