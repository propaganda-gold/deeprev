#ifndef api_invitation_h
#define api_invitation_h 
#include "util/includes.h"
#include "handlers/api.h"

namespace vectorbook {

Unique<HttpHandler> api_invitation_create();
Unique<HttpHandler> api_invitation_list();
Unique<HttpHandler> api_invitation_delete();

Unique<HttpHandler> api_invitation_blacklist_add();

} // namespace vectorbook
#endif /* api_invitation_h */
