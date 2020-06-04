#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "data/connection.h"
#include "data/globals.h"
#include "util/random.h"
#include "data/vectorbook.pb.h"
#include "neural/api.h"
#include "neural/eval.h"
#include "neural/recommendation_publisher.h"
#include "neural/action_converter.h"
#include "neural/boolean_network.h"
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

string GenerateUser() {
	return RandomString("abcd", 2);
}

Void Main() {
	ASSERT_SUCCEEDS(globals::LoadBooleanNetwork("neural/config/model_spec1.pbtxt"));
	ASSERT_SUCCEEDS(CreateRedisConnection("localhost"));
	auto feed = data::unsorted_action_feed();
	ASSERT_OK(feed);

	auto action_map = data::action_map(); ASSERT_OK(action_map);

	ActionConverter converter;

	vector<string> all_actions;
	for (int i = 0; i < 2000; ++i) {
		// Make action.
		Action action;
		action.set_id(Random62String(12));
		// Make context.
		auto context = action.mutable_features();
		context->set_producer(GenerateUser());
		context->set_consumer(GenerateUser());
		// Make label.
		auto label = action.add_label();
		label->set_name("like");
		label->set_value(context->producer()[0] == context->consumer()[0]);

		// Write to DB.
		ASSERT_SUCCEEDS(action_map->put(action.id(), action));
		all_actions.push_back(action.id());
	}

  auto eval = BasicEval();
  auto eval_sampler = BasicSampler();
  while(true) {
		auto idx = RandomInt(all_actions.size());
		feed->append(all_actions[idx]);
		auto item = feed->pop();
		if (item.has_value()) {
			auto action = action_map->const_get(*item); ASSERT_EXISTS(action);

			auto boolean_pair = converter.ConvertProblem(*action);
			ASSERT_OK(boolean_pair);

			auto network_interface = globals::boolean_network_interface();
			ASSERT_OK(network_interface);
			auto network = network_interface->training_network();
			ASSERT_EXISTS(network);

			// auto train_status = network->Train(boolean_pair->request(), boolean_pair->response());

			auto pred_response = network->Train(boolean_pair->request(), boolean_pair->response());
			ASSERT_OK(pred_response);

			eval->Tally(boolean_pair->request(), boolean_pair->response(), *pred_response);
			if (eval_sampler->Tick()) {
				Output_() << "eval->OutputStats(): " << eval->OutputStats() << endl;
			}

		} else {
			std::this_thread::sleep_for (std::chrono::seconds(5));
		}
  }
  std::cout << "Lift off!\n";

  return Ok();
}

#include "util/main_incl.h"
