#include "neural/store_embeddings.h"

#include "data/globals.h"
#include "data/keys.h"
#include "util/includes.h"

namespace vectorbook {

namespace data {
Unique<DoubleVectorMap> embedding_map(const string &embedding_group) {
  return RedisProtoMap<DoubleVector>(StrCat("embedding_map:", embedding_group));
}
}

DoubleVector VectorFromTensor(const Tensor &tensor) {
  DoubleVector result;
  for (size_t i = 0; i < tensor.dim(); ++i) {
    float f = tensor[i].item<float>();
    result.add_weight(f);
  }
  return result;
}

Shared<Tensor> LoadEmbeddingTensor(const Feature &feature) {
  auto embedding_map =
      data::embedding_map(feature.embedding_group());
  auto embedding = embedding_map->const_get(feature.value());
  if (!embedding.has_value()) {
		if (feature.embedding_group() == "consumer") {
		}
		auto dim_size = 8;
    ASSERT_SUCCEEDS(ResetEmbedding(feature.embedding_group(), feature.value(), dim_size));
		embedding = embedding_map->const_get(feature.value());
		ASSERT_EXISTS(embedding);
  }
  auto result = torch::empty({embedding->weight_size()});
  for (int i = 0; i < embedding->weight_size(); ++i) {
    result[i] = embedding->weight(i);
  }
  // TODO(greg) do we need this clone here?
  auto result1 = result.clone();
  result1.set_requires_grad(true);
  return result;
}

StringTensorMap
LoadEmbeddingTensors(const RepeatedPtrField<Feature> &feature_list) {
  StringTensorMap result;
  for (const auto &feature : feature_list) {
    result[feature.name()] = *LoadEmbeddingTensor(feature);
  }
  return result;
}

void SaveEmbeddingTensor(const Feature &feature, const Tensor &tensor) {
  auto embedding_map =
      data::embedding_map(feature.embedding_group());
  embedding_map->put(feature.value(), VectorFromTensor(tensor));
}

Void ResetEmbedding(const string& embedding_group, const string& value, size_t dimension) {
	auto embedding_map = data::embedding_map(embedding_group);
	DoubleVector vector;
	for (size_t i = 0; i < dimension; ++i) {
		int v = rand() % 200 - 100;
		vector.add_weight(0.01 * v);
	}
	embedding_map->put(value, vector);
	return Ok();
}

Option<size_t> EmbeddingSize(const ModelSpec &model_spec,
                            const string &embedding_group) {
  for (const auto &group : model_spec.embedding_group()) {
    if (group.name() == embedding_group) {
      return group.embedding_dim();
    }
  }
  return Null();
}

Option<size_t> EmbeddingConcatSize(const ModelSpec &model_spec) {
  size_t result = 0;
  for (const auto &feature : model_spec.feature_spec()) {
    auto this_size = 8; //EmbeddingSize(model_spec, feature.embedding_group());
    // ASSERT(this_size.has_value());
    result += this_size; // *this_size;
  }
  return result;
}

} // namespace vectorbook
