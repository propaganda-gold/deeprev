#include <sstream>      // std::ostringstream

#include "util/stacktrace.h"
#include <boost/stacktrace.hpp>

namespace vectorbook {

std::string StackTrace() {
	std::ostringstream os;
	os << boost::stacktrace::stacktrace();
	return os.str();
}

} // namespace vectorbook
