#include "util/rate_limiter.h"

#include <ctime>
#include "coln/api.h"
#include "coln/map.h"
#include "data/map.h"
#include "util/includes.h"

namespace vectorbook {
namespace impl {
namespace {

using TokenBucketTallyMap = Map<string, TokenBucketTally>;
TokenBucketTallyMap *GetTallyMap() {
  static Unique<TokenBucketTallyMap> single =
      data::RedisProtoMap<TokenBucketTally>("token_bucket_tally");
  auto r = single.get();
  return r;
}

int GetNowSeconds() {
	std::time_t t = std::time(0);   // get time now
	std::tm* now = std::localtime(&t);
	auto hour = now->tm_hour;
	auto min = now->tm_min;
	auto sec = now->tm_sec;
	auto total_seconds = 60 * 60 * hour + 60 * min + sec;
	return total_seconds;
}
int GetNowBucket(int seconds_per_bucket) {
	auto total_seconds = GetNowSeconds();
	return total_seconds / seconds_per_bucket;
}

} // namespace

// Requires:
//   buckets_per_day >= 1
//   queries_per_bucket >= 1
class TokenBucketLimiter : public RateLimiter {
private:
	TokenBucketOptions options_;
public:
	TokenBucketLimiter(const TokenBucketOptions& options) : options_(options) {}

  Boolean Tally(const string &key) override {
		auto tally = GetTallyMap()->const_get(key); ASSERT_OK(tally);
		if (!tally.has_value()) {
			TokenBucketTally tally;
			tally.set_current_bucket(GetNowBucket(options_.seconds_per_bucket()));
			tally.set_current_tally(1);
			ASSERT_SUCCEEDS(GetTallyMap()->put(key, tally));
			return true;
		}

		// Assume that there is an entry in the tally map.

		auto now_bucket = GetNowBucket(options_.seconds_per_bucket());
		if (tally->current_bucket() == now_bucket) {
      // We are still on the old time quantum. Make sure that we have not used
      // all of our quota.
			if (tally->current_tally() >= options_.queries_per_bucket()) {
				return false;
			} else {
				tally->set_current_tally(tally->current_tally() + 1);
				ASSERT_SUCCEEDS(GetTallyMap()->put(key, *tally));
				return true;
			}
    } else {
      // We have a new time quantum. So, reset the counter.
      tally->set_current_bucket(now_bucket);
      tally->set_current_tally(1);
			ASSERT_SUCCEEDS(GetTallyMap()->put(key, *tally));
			return true;
    }
  }
};

} // namespace impl

Unique<RateLimiter> TokenBucketLimiter(const TokenBucketOptions &options) {
  return new impl::TokenBucketLimiter(options);
}

} // namespace vectorbook
