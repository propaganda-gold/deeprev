#ifndef api_cookietoken_h
#define api_cookietoken_h 
#include "util/includes.h"
#include "handlers/api.h"

namespace vectorbook {

Unique<HttpHandler> api_cookietoken_create();
Unique<HttpHandler> api_cookietoken_validate();
Unique<HttpHandler> api_cookietoken_destroy();

Unique<HttpHandler> debug_cookietoken_create();

} // namespace vectorbook
#endif /* api_cookietoken_h */
