#ifndef api_user_h
#define api_user_h 
#include "util/includes.h"
#include "handlers/api.h"

namespace vectorbook {

Unique<HttpHandler> api_user_create();
Unique<HttpHandler> api_user_destroy();
Unique<HttpHandler> api_user_read();
Unique<HttpHandler> api_user_history();

Unique<HttpHandler> debug_user_create();
Unique<HttpHandler> debug_user_self();

} // namespace vectorbook
#endif /* api_user_h */
