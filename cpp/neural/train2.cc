// This is a debugging training scenario, in which we create an artificial data
// set. Users like posts of others if they have the same first letter of their
// name.
#include <cstdlib>
#include <ctime>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "neural/basic_model.h"
#include "neural/eval.h"
#include "neural/feature_fn.h"
#include "neural/features.pb.h"
#include "neural/inference.h"
#include "neural/store_embeddings.h"
#include "neural/training_config.pb.h"
#include "site/annotations.pb.h"
#include "util/includes.h"

ABSL_FLAG(std::string, training_spec, "", "");
ABSL_FLAG(std::string, model_spec, "", "");
ABSL_FLAG(bool, reset_debug_embeddings, true, "");

using namespace vectorbook;

Void Main() {
  srand(time(0));

  ASSERT_SUCCEEDS(data::LoadModelSpec(absl::GetFlag(FLAGS_model_spec)));
  ASSERT_SUCCEEDS(data::LoadTrainingSpec(absl::GetFlag(FLAGS_training_spec)));

  const TrainingSpec &training_spec = data::training_spec();
  const ModelSpec &model_spec = data::model_spec();

  auto feature_extractor = GetActionFeatures(model_spec.feature_spec());

  std::vector<string> user_ids = {
      "alex",
      "alice",
      "betty",
      "bob",
  };
  // std::vector<string> user_ids = {"alex"};

  if (absl::GetFlag(FLAGS_reset_debug_embeddings)) {
    auto embedding_size = EmbeddingSize(model_spec, "user");
    ASSERT_EXISTS(embedding_size);
    ASSERT_SUCCEEDS(ResetEmbeddings(user_ids, *embedding_size));
  }

  auto model = CreateBasicModel(model_spec);

  auto eval = BasicEval();
  auto eval_sampler = BasicSampler();
  for (int i = 0; i < training_spec.num_outside_iterations(); ++i) {
    size_t user_idx = rand() % user_ids.size();
    size_t user_post_idx = rand() % user_ids.size();
    Action action;
    auto &user_id = user_ids[user_idx];
    auto &author_id = user_ids[user_post_idx];
    action.set_user(user_id);
    action.set_author(author_id);

    BooleanRequest request;
    auto feature_list = feature_extractor.ExtractList(action);
    ASSERT_OK(feature_list);
    *request.mutable_feature_list() = *feature_list;

    BooleanResponse gold_response;
    bool names_match = user_id[0] == author_id[0];
    size_t label = names_match ? 1 : 0;
    gold_response.add_class_prediction()->set_index(label);
    auto pred_response = model->Train(request, gold_response);
    ASSERT_OK(pred_response);

    eval->Tally(request, gold_response, *pred_response);
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
