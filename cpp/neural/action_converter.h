#ifndef neural_action_converter_h
#define neural_action_converter_h
#include "neural/api.h"
#include "util/includes.h"

namespace vectorbook {

Feature CreateFeature(const string &name, const string &value,
                      const string &embedding_group);

class ActionConverter : public ProblemConveter<Action> {
public:
  Option<BooleanRequestAndResponse>
  ConvertProblem(const Action &input) const override {
    BooleanRequestAndResponse result;

		// 1. Make request.
    auto request = result.mutable_request();
    const auto features = input.features();
    ASSERT(features.has_producer());
    *request->add_feature() =
        CreateFeature("producer", features.producer(), "producer");
    ASSERT(features.has_consumer());
    *request->add_feature() =
        CreateFeature("consumer", features.consumer(), "consumer");

		for (const auto& pair : input.label()) {
			request->add_target(pair.name());
		}

		// 2. Make response.
    auto response = result.mutable_response();
		*response->mutable_label() = input.label();

    return result;
  }
  Option<Action>
  ConvertResponse(const BooleanResponse &response) const override {
		Action action;
		*action.mutable_label() = response.label();
		return action;
	}
};

} // namespace vectorbook
#endif /* neural_action_converter_h */
