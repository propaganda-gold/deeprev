#include "neural/basic_model.h"

#include "neural/embedding_layer.h"
#include "neural/store_embeddings.h"
#include "util/includes.h"

namespace vectorbook {
namespace impl {
namespace {

using StringTensorMap = StdMap<string, Tensor>;

struct LocalParameters {
  torch::nn::Linear linear1 = nullptr;
  torch::nn::Linear linear2 = nullptr;
};

class EmbeddingNetwork : public torch::nn::Module {
public:
  void Init() {
    register_module("embedding_layer", embedding_layer_);
    register_module("linear1", locals_.linear1);
    register_module("linear2", locals_.linear2);
  }
  torch::Tensor forward(torch::Tensor x) {
    auto concat = embedding_layer_->forward(x);
    auto e = locals_.linear1->forward(concat);
    auto f = torch::relu(e);
    auto g = locals_.linear2->forward(f);
    return torch::log_softmax(g, 0);
  }

public:
  LocalParameters locals_;
  std::shared_ptr<EmbeddingLayer> embedding_layer_;
};

} // namespace

class BasicModel : public ForwardBackwardModel {
public:
  LocalParameters locals_;
  unique_ptr<torch::optim::SGD> optimizer_;

public:
  Unique<ForwardBundle>
  Forward(const BooleanRequest &request) const override {
    EmbeddingNetwork network;
    network.locals_ = locals_;
		auto layer = EmbeddingLayer::CreateFrom(request.feature());
		ASSERT_OK(layer);
    network.embedding_layer_ = layer.expose();
    network.Init();

    auto bundle = new ForwardBundle();
    bundle->output = network.forward(torch::empty({0}));
    bundle->module_state = network;
    return bundle;
  }

  Unique<BooleanResponse>
  ResponseFromActivation(const Tensor &tensor) const override {
    auto [idx, jdx] = torch::max(tensor, 0);
    auto response = new BooleanResponse();
    response->add_label()->set_value(jdx.item<int>());
    return response;
  }
  Void Backward(const ForwardBundle &bundle,
                const BooleanResponse &gold_response) override {
    optimizer_.reset(new torch::optim::SGD(bundle.module_state.parameters(),
                                           torch::optim::SGDOptions(0.1)));
    optimizer_->zero_grad();
    vector<long> buffer;
    buffer.push_back(gold_response.label(0).value());
    Tensor target = torch::tensor(buffer);
    torch::Tensor loss = torch::nll_loss(bundle.output, target);
    loss.backward();
    optimizer_->step();
				return Ok();
  }

  Void Load(const string &path_name) { return Ok(); }
  Void Save(const string &path_name) const { return Ok(); }
};
} // namespace impl

int kNumOutcomes = 2;

Unique<ForwardBackwardModel> CreateBasicModel(const ModelSpec &model_spec) {
  auto result = new impl::BasicModel();
  auto concat_size = EmbeddingConcatSize(model_spec); ASSERT_OK(concat_size);
  result->locals_.linear1 =
      torch::nn::Linear(*concat_size, model_spec.hidden_dim());
  result->locals_.linear2 =
      torch::nn::Linear(model_spec.hidden_dim(), kNumOutcomes);

  // parameters.embedding_layer =
  // EmbeddingLayer::CreateFrom(request.feature_list().feature());
  // parameters.linear1 = torch::nn::Linear(concat_dim,
  // model_spec.hidden_dim()); parameters.linear2 =
  // torch::nn::Linear(model_spec.hidden_dim(), kNumOutcomes);

  return result;
}

} // namespace vectorbook
