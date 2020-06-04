#include "coln/seq.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Seq, Make) {
	auto seq = MakeSeq<int>();
	seq->append(0);
	seq->append(1);
	seq->append(2);
	seq->append(3);
	for (auto i : *seq) {
	}
}

TEST(Seq, Iterate) {
	auto seq = MakeSeq<int>();
	seq->append(0);
	seq->append(1);
	seq->append(2);
	seq->append(3);
}

} // namespace vectorbook
