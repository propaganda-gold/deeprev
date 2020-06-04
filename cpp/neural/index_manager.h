#ifndef neural_index_manager_h
#define neural_index_manager_h 
#include "util/includes.h"

namespace vectorbook {

// An interface into getting an index for an embedding group.
class IndexInterface {
public:
	// Can lock. Should only be 
	virtual Unique<optional<size_t>> ConstGet(const string& embedding_group,
														const string& value) const = 0;

	virtual Void EnsureExists(const string& embedding_group,
														const string& value) = 0;
};

// Index interface based on Redis.
unique_ptr<IndexInterface> RedisIndexInterface();

} // namespace vectorbook
#endif /* neural_index_manager_h */
