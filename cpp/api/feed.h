#ifndef api_recommendation_h
#define api_recommendation_h 
#include "util/includes.h"
#include "handlers/api.h"

namespace vectorbook {

Unique<HttpHandler> api_feed_single();

} // namespace vectorbook
#endif /* api_recommendation_h */
