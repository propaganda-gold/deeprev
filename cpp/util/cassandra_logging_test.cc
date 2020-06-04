#include "util/cassandra_logging.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Cassandra, Log) {

	auto status = WriteToCassandra("tutorialspoint.test3", "", "");
	ASSERT_TRUE(status.ok());
}

} // namespace vectorbook
