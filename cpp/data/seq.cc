#include "data/seq.h"

#include "util/includes.h"

namespace vectorbook {
namespace data {

unique_ptr<Queue<string>> RedisStringQueue(const string &key) {
  return unique_ptr<Queue<string>>(new impl::RedisStringQueue(key));
}
}
} // namespace vectorbook
