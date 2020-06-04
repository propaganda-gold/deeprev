#ifndef api_apply_h
#define api_apply_h 
#include "util/includes.h"
#include "handlers/api.h"

namespace vectorbook {

Unique<HttpHandler> api_application_create();
Unique<HttpHandler> api_application_verify();

} // namespace vectorbook
#endif /* api_apply_h */
