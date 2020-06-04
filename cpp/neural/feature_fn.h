#ifndef neural_feature_fn_h
#define neural_feature_fn_h
#include "neural/features.pb.h"
#include "neural/training_config.pb.h"
#include "site/annotations.pb.h"
#include "data/vectorbook.pb.h"
#include "util/includes.h"
#include "coln/api.h"

namespace vectorbook {

//
//
// Template part.
template <typename T> class FeatureExtractor {
public:
  virtual string FeatureName() const = 0;
  virtual string EmbeddingGroup() const = 0;
  virtual Unique<string> ExtractValue(const T &t) const = 0;
  Unique<Feature> ExtractPair(const T &t) const {
    auto result = new Feature();
    result->set_name(FeatureName());
    result->set_embedding_group(EmbeddingGroup());
		auto value = ExtractValue(t);
		ASSERT_OK(value);
    result->set_value(*value);
    return result;
  }
};

template <typename T> class FeatureExtractorList {
public:
  using FnPtrList = vector<unique_ptr<FeatureExtractor<T>>>;
  FeatureExtractorList(FnPtrList &&fn_list) : fn_list_(std::move(fn_list)) {}

  Unique<FeatureList> ExtractList(const T &t) const {
    auto result = new FeatureList();
    for (int i = 0; i < fn_list_.size(); ++i) {
			auto pair = fn_list_[i]->ExtractPair(t);
			ASSERT_OK(pair);
      *result->add_feature() = *pair;
    }
    return result;
  }

  size_t size() const { return fn_list_.size(); }

private:
  FnPtrList fn_list_;
};

FeatureExtractorList<Action>
GetActionFeatures(const RepeatedPtrField<FeatureSpec> &feature_specs);

} // namespace vectorbook
#endif /* neural_feature_fn_h */
