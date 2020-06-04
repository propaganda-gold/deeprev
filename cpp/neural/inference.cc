#include "neural/inference.h"

#include "util/includes.h"

namespace vectorbook {
namespace impl {
namespace {

using StringTensorMap = StdMap<string, Tensor>;

int kUserDim = 3;
int kHiddenDim = 4;
int kNumOutcomes = 2;

// Create a proto buffer from a tensor.
DoubleVector VectorFromTensor(const Tensor &tensor) {
  DoubleVector result;
  for (size_t i = 0; i < kUserDim; ++i) {
    float f = tensor[i].item<float>();
    result.add_weight(f);
  }
  return result;
}

Option<Tensor> LoadEmbeddingTensor(const Feature &feature) {
  auto embedding_map = data::embedding_map(feature.embedding_group());
  auto embedding = embedding_map->const_get(feature.value());
	ASSERT_OK(embedding);
  auto result = torch::empty({kUserDim});
  for (int i = 0; i < embedding->weight_size(); ++i) {
    result[i] = embedding->weight(i);
  }
  auto result1 = result.clone();
  result1.set_requires_grad(true);
  return result;
}

void SaveEmbeddingTensor(const Feature &feature, const Tensor &tensor) {
  auto embedding_map = data::embedding_map(feature.embedding_group());
  embedding_map->put(feature.value(), VectorFromTensor(tensor));
}

class EmbeddingNetwork : public torch::nn::Module {
public:
  EmbeddingNetwork(const torch::Tensor &action_embed,
                   const torch::Tensor &post_embed,
                   const torch::nn::Linear &linear1,
                   const torch::nn::Linear &linear2)
      : action_embed_(action_embed), post_embed_(post_embed), linear1_(linear1),
        linear2_(linear2) {
    register_parameter("action_embed", action_embed_);
    register_parameter("post_embed", post_embed_);
    register_module("linear1", linear1_);
    register_module("linear2", linear2_);
  }
  torch::Tensor forward(torch::Tensor x) {
		auto ones = torch::eye(1);
		auto i1 = action_embed_.mul(ones);
		auto i2 = post_embed_.mul(ones);
    Tensor concat = torch::flatten(torch::cat({i1, i2}));
    auto e = linear1_->forward(concat);
    auto f = torch::relu(e);
    auto g = linear2_->forward(f);
    return torch::log_softmax(g, 0);
  }

  torch::Tensor action_embed_;
  torch::Tensor post_embed_;
  torch::nn::Linear linear1_;
  torch::nn::Linear linear2_;
};

} // namespace

class TorchModel : public TrainableModel {
public:
  Unique<BooleanResponse> Predict(const BooleanRequest &request) const {
    return Ok();
  }
  Unique<BooleanResponse>
  Train(const BooleanRequest &request,
        const BooleanResponse &gold_response) override {
    // Check inputs.
    ASSERT(gold_response.class_prediction_size() == 1);
    for (const auto &feature : request.feature_list().feature()) {
      if (feature.name() == "action_author") {
				auto vector = LoadEmbeddingTensor(feature); ASSERT_OK(vector);
        action_embed_ = *vector;
      } else if (feature.name() == "post_author") {
				auto vector = LoadEmbeddingTensor(feature); ASSERT_OK(vector);
        post_embed_ = *vector;
      } else {
        throw StrCat("Couldn't find the feature: ", feature.ShortDebugString());
      }
    }
    // Run the network.
    EmbeddingNetwork network(action_embed_, post_embed_, linear1_, linear2_);

    optimizer_.reset(new torch::optim::SGD(network.parameters(),
                                           torch::optim::SGDOptions(0.01)));

    optimizer_->zero_grad();
    auto output = network.forward(torch::tensor({1L, 1L}));
    // Compute the loss.
    vector<long> buffer;
    buffer.push_back(gold_response.class_prediction(0).index());
    Tensor target = torch::tensor(buffer);
    torch::Tensor loss = torch::nll_loss(output, target);
    loss.backward();
    // Step.
    for (auto &kv : network.named_parameters()) {
      if (kv.key() == "action_embed") {
      }
    }
    optimizer_->step();
    for (auto &kv : network.named_parameters()) {
      if (kv.key() == "action_embed") {
      }
    }
    for (const auto &feature : request.feature_list().feature()) {
      if (feature.name() == "action_author") {
        SaveEmbeddingTensor(feature, action_embed_);
      } else if (feature.name() == "post_author") {
        SaveEmbeddingTensor(feature, post_embed_);
      } else {
        throw StrCat("Couldn't find the feature: ", feature.ShortDebugString());
      }
    }
    // network_.SaveEmbeddings(request.feature_list());
    // Create response.
    auto [idx, jdx] = torch::max(output, 0);
    auto response = new BooleanResponse();
    response->add_class_prediction()->set_index(jdx.item<int>());
    return response;
  }
  Void Load(const string &path_name) {
    action_embed_ = torch::empty({1, kUserDim});
    post_embed_ = torch::empty({1, kUserDim});
    linear1_ = torch::nn::Linear(2 * kUserDim, kHiddenDim);
    linear2_ = torch::nn::Linear(kHiddenDim, kNumOutcomes);
    return Ok();
  }
  Void Save(const string &path_name) const {
    //	        torch::save(net, "net.pt");
    return Ok();
  }

private:
  Tensor action_embed_;
  Tensor post_embed_;
  torch::nn::Linear linear1_ = nullptr;
  torch::nn::Linear linear2_ = nullptr;

  unique_ptr<torch::optim::SGD> optimizer_;
};
} // namespace impl

unique_ptr<PredictionModel> TorchModel() {
  return unique_ptr<PredictionModel>(new impl::TorchModel());
}

} // namespace vectorbook
