#include "neural/example_generator.h"

#include "coln/util.h"
#include "data/globals.h"
#include "neural/feature_fn.h"
#include "neural/training_config.pb.h"
#include "util/includes.h"

namespace vectorbook {
namespace {

using Predictions = RepeatedPtrField<ClassPrediction>;

Option<int> IndexFor(const OutputSpace &space, const string &value) {
  for (size_t i = 0; i < space.value().size(); ++i) {
    const string &test_value = space.value(i);
    if (test_value == value) {
      return i;
    }
  }
  return Null();
}
Unique<Predictions> ActionToPredictions(const Action &action,
                                        const ModelSpec &spec) {
  auto predictions = new Predictions();
	{
  	const auto &pair = action.label();
    const string &problem_name = pair.name();
    const string &gold_value = pair.value();

    auto prediction = predictions->Add();
    prediction->set_name(problem_name);
    for (const OutputSpace &space : spec.output_space()) {
      if (space.name() == problem_name) {
        auto index = IndexFor(space, gold_value);
        ASSERT_OK(index);
        ASSERT(index.has_value());
        prediction->set_index(*index);
      }
    }
  }
  return predictions;
}

} // namespace

using ExampleGenerator;
namespace impl {

class ActionGenerator : public ExampleGenerator {
public:
  ActionGenerator(FeatureExtractorList<Action> &&feature_extractor)
      : feature_extractor_(std::move(feature_extractor)) {}

  Unique<PredictionPair> RandomExample() const override {
    // Pick a random action.
    auto action = RandomFromIterable<KV<string, Action>>(
        *data::action_map());
    ASSERT_OK(action);

    // Make the pair.
    auto result = new PredictionPair();
    auto request = RequestFromAction(action->value());
    ASSERT_OK(request);
    *result->mutable_request() = *request;
    auto response = ResponseFromAction(action->value());
    ASSERT_OK(response);
    *result->mutable_response() = *response;
    return result;
  }

  FeatureExtractorList<Action> feature_extractor_;
  ModelSpec model_spec_;

private:
  Unique<BooleanRequest> RequestFromAction(const Action &action) {
    auto result = new BooleanRequest();
    auto list = feature_extractor_.ExtractList(action);
    ASSERT_OK(list);
    *result->mutable_feature_list() = *list;
    return result;
  }
  Unique<BooleanResponse> ResponseFromAction(const Action &action) {
    auto result = new BooleanResponse();
    auto predictions = ActionToPredictions(action, model_spec_);
    ASSERT_OK(predictions);
    *result->mutable_class_prediction() = *predictions;
    return result;
  }
};

} // namespace impl

Unique<ExampleGenerator> ActionGenerator(const ModelSpec &model_spec) {
  auto ptr =
      new impl::ActionGenerator(GetActionFeatures(model_spec.feature_spec()));
  ptr->model_spec_ = model_spec;
  return Unique<ExampleGenerator>(ptr);
}

} // namespace vectorbook
