#ifndef api_message_h
#define api_message_h 
#include "util/includes.h"
#include "handlers/api.h"

namespace vectorbook {

Unique<HttpHandler> api_broadcast_create();
Unique<HttpHandler> api_broadcast_read();
Unique<HttpHandler> api_broadcast_delete();

} // namespace vectorbook
#endif /* api_message_h */
