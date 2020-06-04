#include "neural/feature_fn.h"

#include "util/includes.h"
#include "data/globals.h"

namespace vectorbook {
namespace {

// Indicates the identity of the author of the *action*.
class ActionAuthorFeature : public FeatureExtractor<Action> {
public:
  string FeatureName() const override { return "action_author"; }
  string EmbeddingGroup() const { return "user"; }
  Unique<string> ExtractValue(const Action &t) const override { return new string(t.user()); }
};

// Indicates the identity of the author of the *broadcast*.
class PostAuthorFeature : public FeatureExtractor<Action> {
public:
  string FeatureName() const override { return "post_author"; }
  string EmbeddingGroup() const { return "user"; }
  Unique<string> ExtractValue(const Action &t) const override {
		return new string(t.author());
  }
};

FeatureExtractor<Action> *FromName(const string &name) {
  if (name == "action_author") {
    return new ActionAuthorFeature();
	}
  if (name == "post_author") {
    return new PostAuthorFeature();
	}
  return nullptr;
}

} // namespace

FeatureExtractorList<Action>
GetActionFeatures(const RepeatedPtrField<FeatureSpec>& feature_specs) {
  vector<unique_ptr<FeatureExtractor<Action>>> buffer;
  for (const auto &spec : feature_specs) {
    buffer.push_back(unique_ptr<FeatureExtractor<Action>>(FromName(spec.name())));
  }
  return FeatureExtractorList<Action>(std::move(buffer));
}

} // namespace vectorbook
