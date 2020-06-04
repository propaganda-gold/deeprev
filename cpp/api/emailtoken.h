#ifndef api_emailtoken_h
#define api_emailtoken_h 
#include "util/includes.h"
#include "handlers/api.h"

namespace vectorbook {

Unique<HttpHandler> api_emailtoken_create();
Unique<HttpHandler> api_emailtoken_validate();

} // namespace vectorbook
#endif /* api_emailtoken_h */
