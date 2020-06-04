#include "util/logging.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Output, Stream) {
	Output_() << "hello there" << std::endl;
}

} // namespace vectorbook
