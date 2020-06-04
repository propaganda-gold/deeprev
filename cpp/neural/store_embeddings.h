#ifndef neural_store_embeddings_h
#define neural_store_embeddings_h
#include "coln/api.h"
#include "data/vectorbook.pb.h"
#include "neural/features.pb.h"
#include "neural/training_config.pb.h"
#include "site/annotations.pb.h"
#include "torch/torch.h"
#include "util/includes.h"

namespace vectorbook {
namespace data {
using DoubleVectorMap = Map<string, DoubleVector>;
Unique<DoubleVectorMap> embedding_map(const string &embedding_group) ;
}
using torch::Tensor;

using StringTensorMap = unordered_map<string, Tensor>;

// Create a proto buffer from a tensor.
DoubleVector VectorFromTensor(const Tensor &tensor);

// Will create a new tensor in the DB if not found.
Shared<Tensor> LoadEmbeddingTensor(const Feature &feature, int dim_size);
Shared<Tensor> LoadEmbeddingTensor(const Feature &feature);

StringTensorMap
LoadEmbeddingTensors(const RepeatedPtrField<Feature> &feature_list);

void SaveEmbeddingTensors(const RepeatedPtrField<Feature> &feature,
                          const StringTensorMap &tensor);

Void ResetEmbedding(const string& embedding_group, const string& value, size_t dimension);

Option<size_t> EmbeddingSize(const ModelSpec &model_spec,
                               const string &embedding_group);

Option<size_t> EmbeddingConcatSize(const ModelSpec &model_spec);

} // namespace vectorbook
#endif /* neural_store_embeddings_h */
