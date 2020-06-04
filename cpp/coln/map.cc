#include "coln/map.h"

namespace vectorbook {

std::function<int()> EmptyInt() {
	return []() { return 0; };
}

std::function<float()> EmptyFloat() {
	return []() { return 0.0; };
}

} // namespace vectorbook
