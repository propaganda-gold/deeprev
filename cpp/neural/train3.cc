// This is a debugging training scenario, in which we create an artificial data
// set. Users like posts of others if they have the same first letter of their
// name.
#include <cstdlib>
#include <ctime>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "neural/api.h"
#include "neural/basic_model.h"
#include "neural/eval.h"
#include "neural/feature_fn.h"
#include "neural/features.pb.h"
#include "neural/store_embeddings.h"
#include "neural/training_config.pb.h"
#include "site/annotations.pb.h"
#include "util/includes.h"

ABSL_FLAG(std::string, config_file, "", "Type of the class, in {lib, bin}.");
ABSL_FLAG(bool, reset_debug_embeddings, false, "For debugging: .");

using namespace vectorbook;

namespace {

class RandomPairsGenerator : public RequestGenerator {
public:
  RandomPairsGenerator(const FeatureExtractorList<Action> *feature_extractor)
      : feature_extractor_(feature_extractor) {}

  BooleanRequest
  RandomRequest(const RequestRequest &request) const override {

    Action action;
    std::vector<string> user_ids = {
        "alex",
        "alice",
        "betty",
        "bob",
    };
    size_t user_idx = rand() % user_ids.size();
    size_t user_post_idx = rand() % user_ids.size();
    auto &user_id = user_ids[user_idx];
    auto &user_post_id = user_ids[user_post_idx];
    action.set_user_id(user_id);
    action.set_post_user_id(user_post_id);

    BooleanRequest result;
    *result.mutable_feature_list() = feature_extractor_->ExtractList(action);
    return result;
  }

private:
  const FeatureExtractorList<Action> *feature_extractor_;
};

class SameLetterOracle : public PredictionModel {
public:
  Unique<BooleanResponse> Predict(const BooleanRequest &request) const {
		assert(request.feature_list().feature_size() == 2);
    BooleanResponse response;
		auto& feature0 = request.feature_list().feature(0).value();
		auto& feature1 = request.feature_list().feature(1).value();
    bool names_match = feature0 == feature1;
    size_t label = names_match ? 1 : 0;
    response.add_class_prediction()->set_prediction_index(label);
    return response;
  }
};

} // namespace

int main(int argc, char **argv) {
  srand(time(0));
  absl::ParseCommandLine(argc, argv);

  vectorbook::TrainingSpec config;
  ReadProto(absl::GetFlag(FLAGS_config_file), &config);
  Output_() << "config: " << config.DebugString() << endl;

  if (absl::GetFlag(FLAGS_reset_debug_embeddings)) {
		std::vector<string> user_ids = {"alex",  "alice",   "betty", "bob", };
		auto embedding_size = EmbeddingSize(config.model_spec(), "user");
		assert(embedding_size.has_value());
    ResetEmbeddings(user_ids, *embedding_size);
  }


  FeatureExtractorList<Action> feature_extractor =
      GetActionFeatures(config.model_spec().feature_spec());

  auto model = CreateBasicModel(config.model_spec());
  auto request_generator =
      absl::make_unique<RandomPairsGenerator>(&feature_extractor);
  auto artificial_oracle = absl::make_unique<SameLetterOracle>();
  // model->Load("");

  auto eval = BasicEval();
  auto eval_sampler = BasicSampler();
  for (int i = 0; i < config.num_outside_iterations(); ++i) {
    RequestRequest request_request;
    auto request = request_generator->RandomRequest(request_request);
    auto gold_response = artificial_oracle->Predict(request).ValueOrDie();
    auto pred_response = model->Train(request, gold_response).ValueOrDie();
    eval->Tally(request, gold_response, pred_response);
    if (eval_sampler->Tick()) {
      Output_() << "eval->OutputStats(): " << eval->OutputStats() << endl;
    }
    // break;
  }
  return 0;
}
