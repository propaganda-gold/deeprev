// This is a debugging training scenario, in which we create an artificial data
// set. Users like posts of others if they have the same first letter of their
// name.
#include <cstdlib>
#include <ctime>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "neural/eval.h"
#include "neural/feature_fn.h"
#include "neural/features.pb.h"
#include "neural/inference.h"
#include "neural/basic_model.h"
#include "neural/training_config.pb.h"
#include "site/annotations.pb.h"
#include "util/includes.h"

ABSL_FLAG(std::string, config_file, "", "Type of the class, in {lib, bin}.");
ABSL_FLAG(bool, reset_debug_embeddings, false, "For debugging: .");

using namespace vectorbook;

void ResetEmbeddings(vector<string> user_ids) {
  for (string user_id : user_ids) {
    auto embedding_map = data::embedding_map("user");
    DoubleVector vector;
    for (int i = 0; i < 3; ++i) {
      int v = rand() % 100 - 50;
      vector.add_weight(0.002 * v);
    }
    embedding_map->put(user_id, vector);
  }
}

Void Main() {
  srand(time(0));
  auto config2 = ReadProto<TrainingSpec>(absl::GetFlag(FLAGS_config_file));
	ASSERT_OK(config2);
	auto& config = *config2;
  Output_() << "config: " << config.DebugString() << endl;

  auto feature_extractor =
      GetActionFeatures(config.model_spec().feature_spec());

  std::vector<string> user_ids = {"alex",  "alice",   "betty", "bob", };
  // std::vector<string> user_ids = {"alex"};

  if (absl::GetFlag(FLAGS_reset_debug_embeddings)) {
    ResetEmbeddings(user_ids);
  }

  auto model = TorchModel(config);
  // model->Load("");

  auto eval = BasicEval();
  auto eval_sampler = BasicSampler();
  for (int i = 0; i < config.num_outside_iterations(); ++i) {
    size_t user_idx = rand() % user_ids.size();
    size_t user_post_idx = rand() % user_ids.size();
    Action action;
    auto &user_id = user_ids[user_idx];
    auto &user_post_id = user_ids[user_post_idx];
    action.set_user(user_id);
    action.set_object(user_post_id);

    BooleanRequest request;
    *request.mutable_feature_list() = feature_extractor.Extract(action);

    BooleanResponse gold_response;
    bool names_match = user_id[0] == user_post_id[0];
    size_t label = names_match ? 1 : 0;
    gold_response.add_class_prediction()->set_index(label);
    auto pred_response = model->Train(request, gold_response).ValueOrDie();

    eval->Tally(request, gold_response, pred_response);
    if (eval_sampler->Tick()) {
      Output_() << "eval->OutputStats(): " << eval->OutputStats() << endl;
    }
  }

  return Ok();
}

int main(int argc, char **argv) {
	absl::ParseCommandLine(argc, argv);
	Void result = Main();
	if (result.has_error()) {
		Output_() << "Error:\n";
		Output_() << result.error().StackTrace() << endl;
	}
	return 0;
}
