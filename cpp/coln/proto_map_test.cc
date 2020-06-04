#include "coln/proto_map.h"

#include "data/vectorbook.pb.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(RepeatedEnumMap, Basic) {
  RepeatedPtrField<LabelPair> labels;
  RepeatedEnumMap<LabelPair, NameEnum, ValueEnum> map(&labels,
                                                      EmptyProto<ValueEnum>());
  map.put(NameEnum::AGREE, ValueEnum::POS_ONE);
  map.put(NameEnum::AGREE, ValueEnum::POS_ONE);
  map.put(NameEnum::AGREE, ValueEnum::NEG_ONE);
  map.put(NameEnum::QUALITY, ValueEnum::NEG_ONE);

  auto it = map.mutable_get(NameEnum::CREDIBLE);
  ASSERT_TRUE(it.ok());
  ASSERT_TRUE(it.has_value());
  *it = ValueEnum::POS_ONE;
  it.write();
}

TEST(RepeatedProtoMap, Basic) {
  RepeatedPtrField<LabelTallyPair> labels;
  RepeatedProtoMap<LabelTallyPair, NameEnum, LabelTally> map(
      &labels, EmptyProto<LabelTally>());
  map.put(NameEnum::AGREE, LabelTally());
	auto it = map.mutable_get(NameEnum::CREDIBLE);
	ASSERT_TRUE(it.ok());
	ASSERT_TRUE(it.has_value());
	*it = LabelTally();
	it.write();
}

} // namespace vectorbook
