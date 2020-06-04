#include <cstdlib>
#include <ctime>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "data/globals.h"
#include "neural/api.h"
#include "neural/basic_model.h"
#include "neural/eval.h"
#include "neural/feature_fn.h"
#include "neural/features.pb.h"
#include "neural/store_embeddings.h"
#include "neural/example_generator.h"
#include "neural/training_config.pb.h"
#include "site/annotations.pb.h"
#include "util/includes.h"
#include <boost/stacktrace.hpp>

ABSL_FLAG(std::string, training_spec, "", "");
ABSL_FLAG(std::string, model_spec, "", "");

using namespace vectorbook;

Void Main() {
	srand(time(0));
	data::LoadModelSpec(absl::GetFlag(FLAGS_model_spec));
	data::LoadTrainingSpec(absl::GetFlag(FLAGS_training_spec));

	const TrainingSpec &training_spec = data::training_spec();
	const ModelSpec &model_spec = data::model_spec();

	auto model = CreateBasicModel(model_spec);
	auto generator = ActionGenerator(model_spec);
	ASSERT_OK(generator);
	auto eval = BasicEval();
	auto eval_sampler = BasicSampler();

	for (int i = 0; i < training_spec.num_outside_iterations(); ++i) {
		auto pair = generator->RandomExample();
		ASSERT_OK(pair);
		auto pred_response = model->Train(pair->request(), pair->response());
		ASSERT_OK(pred_response);
		eval->Tally(pair->request(), pair->response(), *pred_response);
		if (eval_sampler->Tick()) {
			Output_() << "eval->OutputStats(): " << eval->OutputStats() << endl;
		}
		// break;
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
