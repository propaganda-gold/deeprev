#ifndef api_channel_h
#define api_channel_h 
#include "util/includes.h"

namespace vectorbook {

Unique<HttpHandler> api_channel_create();
Unique<HttpHandler> api_channel_destroy();
Unique<HttpHandler> api_channel_profile();

Unique<HttpHandler> api_channel_invite();

// This will either 1) remove an open invitation, or 2) remove an active
// member.
Unique<HttpHandler> api_channel_disinvite();

Unique<HttpHandler> api_channel_accept();
Unique<HttpHandler> api_channel_leave();


} // namespace vectorbook
#endif /* api_channel_h */
