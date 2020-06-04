#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "data/connection.h"
#include "data/globals.h"
#include "data/vectorbook.pb.h"
#include "neural/api.h"
#include "neural/recommendation_publisher.h"
#include "util/includes.h"
#include "server/options.pb.h"
#include <boost/filesystem.hpp>
#include <memory>
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

ABSL_FLAG(std::string, http_options, "", "");
ABSL_FLAG(std::string, model_spec, "", "");
ABSL_FLAG(int, port, 8080, "The port number for the server.");

using namespace std;
using namespace vectorbook;

Void Main() {
	ASSERT_SUCCEEDS(CreateRedisConnection("localhost"));
	auto feed = data::unsorted_post_feed();
	ASSERT_OK(feed);
	for (int i = 0; i < 3; ++i) {
		auto status = feed->append(StrCat("item", i));
		ASSERT_OK(status);
	}

	auto sorter = RecommendationPublisher::Create(PublisherOptions());
	ASSERT_OK(sorter);

  while(true) {
		auto item = feed->pop();
		if (item.has_value()) {
			auto status = sorter->Process(*item);
			ASSERT_OK(status);
		} else {
			std::this_thread::sleep_for (std::chrono::seconds(5));
		}
  }
  std::cout << "Lift off!\n";

  return Ok();
}

#include "util/main_incl.h"
