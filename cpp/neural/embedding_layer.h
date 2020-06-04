#ifndef neural_embedding_layer_h
#define neural_embedding_layer_h
#include "coln/api.h"
#include "coln/map.h"
#include "neural/api.h"
#include "neural/store_embeddings.h"
#include "neural/training_config.pb.h"
#include "torch/torch.h"
#include "util/includes.h"

namespace vectorbook {

using StringVector = vector<string>;

class EmbeddingLayer : public torch::nn::Module {
public:
  static Shared<EmbeddingLayer>
  CreateFrom(const RepeatedPtrField<Feature> &feature_list) {
    shared_ptr<EmbeddingLayer> result(new EmbeddingLayer());
    size_t concat_dim = 0;
    for (int i = 0; i < feature_list.size(); ++i) {
      auto &feature = feature_list[i];
      result->embedding_names_.push_back(feature.name());
			auto tensor = LoadEmbeddingTensor(feature);
			ASSERT_OK(tensor);
      result->name_embedding_map_[feature.name()] = *tensor;
    }
    return result;
  }

  // NOTE(greg) We just ignore 'x'.
  torch::Tensor forward(torch::Tensor x) {
    vector<Tensor> embeddings;
    for (int i = 0; i < embedding_names_.size(); ++i) {
      auto &embedding_name = embedding_names_[i];
      auto &embedding_tensor = name_embedding_map_[embedding_name];
      auto ones = torch::eye(1);
      auto part = embedding_tensor.mul(ones);
      embeddings.push_back(part);
    }
    return torch::flatten(torch::cat(embeddings));
  }
  const StringVector &embedding_names() const { return embedding_names_; }
  const StringTensorMap &name_embedding_map() const {
    return name_embedding_map_;
  }

private:
  void RegisterParameters() {
    for (const string &embedding_name : embedding_names_) {
      register_parameter(embedding_name, name_embedding_map_[embedding_name]);
    }
  }
  vector<string> embedding_names_;
  StringTensorMap name_embedding_map_;
};

} // namespace vectorbook
#endif /* neural_embedding_layer_h */
