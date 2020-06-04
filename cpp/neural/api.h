#ifndef neural_api_h
#define neural_api_h
#include "coln/api.h"
#include "data/vectorbook.pb.h"
#include "neural/features.pb.h"
#include "torch/torch.h"
#include "util/includes.h"

namespace vectorbook {
using torch::Tensor;
using torch::nn::Module;

// Assumes Torch.
struct ForwardBundle {
  Tensor output;
  Module module_state;
};

// Converts domain-specific problem type to network.
template <typename T> class ProblemConveter {
public:
  virtual Option<BooleanRequestAndResponse>
  ConvertProblem(const T &input) const = 0;
  virtual Option<T> ConvertResponse(const BooleanResponse &input) const = 0;
};

class PredictionModel {
public:
  // Make a prediction.
  virtual Unique<BooleanResponse>
  Predict(const BooleanRequest &request) const = 0;
};

class TrainableModel : public PredictionModel {
public:
  // Train on one example.
  virtual Unique<BooleanResponse>
  Train(const BooleanRequest &request,
        const BooleanResponse &gold_response) = 0;
};

// Assumes torch, by including ForwardBundle.
class ForwardBackwardModel : public TrainableModel {
public:
  //
  // Derived functions.
  Unique<BooleanResponse>
  Predict(const BooleanRequest &request) const override {
    auto forward_result = Forward(request);
    ASSERT_OK(forward_result);
    return ResponseFromActivation(forward_result->output);
  }

  Unique<BooleanResponse> Train(const BooleanRequest &request,
                                const BooleanResponse &gold_response) override {
    // Forward.
    auto forward_bundle = Forward(request);
    ASSERT_OK(forward_bundle);
    // Forward.

    // Backward.
    ASSERT_SUCCEEDS(Backward(*forward_bundle, gold_response));
    // Forward.

    // Response.
    auto response = ResponseFromActivation(forward_bundle->output);
    ASSERT_OK(response);
    // Forward.
    return response;
  }

  //
  // Pure virtuals.

  // Prediction mode.
  virtual Unique<ForwardBundle>
  Forward(const BooleanRequest &request) const = 0;

  virtual Unique<BooleanResponse>
  ResponseFromActivation(const Tensor &tensor) const = 0;

  // Training mode.
  // 1) Parameters should be updated in place.
  // 2) The predicted value should be returned for loss calculation outside.
  // 3) Sub-class should fail if it does not think 'response' is a valid output
  // for 'request'.
  virtual Void Backward(const ForwardBundle &output,
                        const BooleanResponse &gold_response) = 0;
};

DoubleVector RandomVector(size_t dimension);

} // namespace vectorbook
#endif /* neural_api_h */
