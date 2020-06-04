#include "site/annotations.pb.h"
#include "neural/feature_fn.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(FeatureExtractorList, Basic) {
	FeatureExtractorList<Action>::FnPtrList ptr_list;
	FeatureExtractorList<Action> list(std::move(ptr_list));
}

} // namespace vectorbook
