#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "data/connection.h"
#include "data/globals.h"
#include "data/vectorbook.pb.h"
#include "neural/action_converter.h"
#include "neural/api.h"
#include "neural/boolean_network.h"
#include "neural/eval.h"
#include "neural/recommendation_publisher.h"
#include "server/options.pb.h"
#include "util/includes.h"
#include "util/random.h"
#include <boost/filesystem.hpp>
#include <chrono>   // std::chrono::seconds
#include <iostream> // std::cout, std::endl
#include <memory>
#include <thread> // std::this_thread::sleep_for

ABSL_FLAG(std::string, http_options, "", "");
ABSL_FLAG(std::string, model_spec, "", "");
ABSL_FLAG(int, port, 8080, "The port number for the server.");

using namespace std;
using namespace vectorbook;

namespace {

Void HandleBroadcastEvent(const string &id) { return Ok(); }

Void HandleActionEvent(const string &id) {
  auto action_map = data::action_map();
  ASSERT_OK(action_map);
  auto action = action_map->const_get(id);
  ASSERT_EXISTS(action);
  ActionConverter converter;
  auto boolean_pair = converter.ConvertProblem(*action);
  ASSERT_OK(boolean_pair);

  auto network_interface = globals::boolean_network_interface();
  ASSERT_OK(network_interface);
  auto network = network_interface->training_network();
  ASSERT_EXISTS(network);

  // auto train_status = network->Train(boolean_pair->request(),
  // boolean_pair->response());

  auto pred_response =
      network->Train(boolean_pair->request(), boolean_pair->response());
  ASSERT_OK(pred_response);

  return Ok();
}

Void HandleMessageEvent(const string &id) { return Ok(); }

} // namespace

Void Main() {
  ASSERT_SUCCEEDS(globals::LoadBooleanNetwork("neural/config/model_spec1.pbtxt"));
  ASSERT_SUCCEEDS(CreateRedisConnection("localhost"));

  auto broadcast_queue = data::raw_broadcast_queue();
  ASSERT_OK(broadcast_queue);
  auto action_queue = data::raw_action_queue();
  ASSERT_OK(action_queue);
  auto message_queue = data::raw_message_queue();
  ASSERT_OK(message_queue);

  while (true) {
    bool did_something = false;
    {
      auto item = broadcast_queue->pop();
      if (item.has_value()) {
        ASSERT_SUCCEEDS(HandleBroadcastEvent(*item));
        did_something = true;
      }
    }
    {
      auto item = action_queue->pop();
      if (item.has_value()) {
        ASSERT_SUCCEEDS(HandleActionEvent(*item));
        did_something = true;
      }
    }
    {
      auto item = message_queue->pop();
      if (item.has_value()) {
        ASSERT_SUCCEEDS(HandleMessageEvent(*item));
        did_something = true;
      }
    }
    if (!did_something) {
      Output_msg("Sleeping for 10s.");
      std::this_thread::sleep_for(std::chrono::seconds(10));
    }
  }

  Output_msg("Lift off!");

  return Ok();
}

#include "util/main_incl.h"
