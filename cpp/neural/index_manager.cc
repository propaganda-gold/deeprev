#include "neural/index_manager.h"
#include "absl/strings/numbers.h"
#include "data/globals.h"

#include "util/includes.h"

namespace vectorbook {
namespace impl {
class RedisIndexInterface : public IndexInterface {
public:
  // This interface abstracts how
  Unique<optional<size_t>> ConstGet(const string &embedding_group,
                                      const string &value) const override {
    StringStringMap *user_map = data::BigEmbeddingMap(embedding_group);
    optional<string> result = user_map->get(value);
		if (!result) {
			return Unique<optional<size_t>>(absl::nullopt);
		} else {
			size_t int_result;
			ASSERT(absl::SimpleAtoi(*result, &int_result));
			return Unique<optional<size_t>>(int_result);
		}
  }
	// TODO(greg) Make this thread safe!
  Void EnsureExists(const string &embedding_group,
                      const string &value) override {
    StringStringMap *user_map = data::BigEmbeddingMap(embedding_group);
    optional<string> result = user_map->get(value);
		if (!result) {
			size_t size = user_map->size();
			user_map->put(value, StrCat(size));
		}
    return Ok();
  }
};
} // namespace impl

unique_ptr<IndexInterface> RedisIndexInterface() { return unique_ptr<IndexInterface>(new impl::RedisIndexInterface()); }
} // namespace vectorbook
