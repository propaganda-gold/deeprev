#ifndef UTIL_INCLUDES
#define UTIL_INCLUDES
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "absl/types/optional.h"
#include "absl/memory/memory.h"
#include "util/logging.h"
#include "util/strings.h"
#include "util/files.h"

#include "coln/api.h"
#include "coln/map.h"
#include "coln/seq.h"
#include "coln/set.h"

#include "google/protobuf/map.h"
#include "google/protobuf/stubs/common.h"
#include "google/protobuf/stubs/logging.h"
#include "google/protobuf/stubs/status_macros.h"
#include "google/protobuf/stubs/statusor.h"
#include "google/protobuf/testing/file.h"
#include "google/protobuf/text_format.h"

namespace vectorbook {
// std using statements.
using std::endl;
using std::pair;
using std::function;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// optional.
using absl::nullopt;
using absl::optional;

// protobuf using statements.
using google::protobuf::TextFormat;
using google::protobuf::RepeatedPtrField;

time_t CurrentTimeInfo();

} // namespace vectorbook
#endif /* UTIL_INCLUDES */
