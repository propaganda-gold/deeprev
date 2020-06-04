#include "data/seq.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {
namespace data {

TEST(Queue, Make) {
  auto seq = RedisStringQueue("seq1");
  seq->append("a");
  seq->append("b");
  for (auto s : *seq) {
  }
}

} // namespace data
} // namespace vectorbook
