#include <chrono>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "util/includes.h"
#include "util/rate_limiter.h"

using namespace vectorbook;

Void Main() {
	TokenBucketOptions options;
	options.set_seconds_per_bucket(1);
	options.set_queries_per_bucket(10);
	auto limiter = TokenBucketLimiter(options); ASSERT_OK(limiter);

	for (int i = 0; i < 1000; ++i) {
		Output_value(i);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		auto result = limiter->Tally("0"); ASSERT_OK(result);
	}


	return Ok();
}

#include "util/main_incl.h"
