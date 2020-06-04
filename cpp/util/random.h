#ifndef util_random_h
#define util_random_h 

#include <vector>
#include <string>
#include "coln/api.h"

namespace vectorbook {

std::string RandomString(const std::string &characters, int length);
std::string Random62String(size_t length);
std::string Random62String();

size_t RandomInt(size_t N);

// Returns true if a random number in [0, 1) is <= point.
Unique<bool> RandomTestFloat(float point);

template <typename T> const T& RandomFromVector(const std::vector<T>& v) {
	size_t idx = RandomInt(v.size());
	return v[idx];
}

} // namespace vectorbook
#endif /* util_random_h */
