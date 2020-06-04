#ifndef api_resource_h
#define api_resource_h 
#include "util/includes.h"
#include "handlers/api.h"

namespace vectorbook {

// get_or_update
Unique<HttpHandler> api_resource_get();

} // namespace vectorbook
#endif /* api_resource_h */
