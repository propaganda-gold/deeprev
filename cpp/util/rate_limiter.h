#ifndef util_rate_limiter_h
#define util_rate_limiter_h 
#include "util/includes.h"
#include "util/rate_limiter_options.pb.h"

namespace vectorbook {

class RateLimiter {
public:
	virtual ~RateLimiter() {}

	// "tallies" one tick for 'key'. Returns true iff 'key' is allowed to issue a
	// request.
	virtual Boolean Tally(const string& key) = 0;
};

Unique<RateLimiter> TokenBucketLimiter(const TokenBucketOptions& options);

} // namespace vectorbook
#endif /* util_rate_limiter_h */
