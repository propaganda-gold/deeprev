#include <dlfcn.h>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "util/includes.h"
#include <boost/stacktrace.hpp>

using namespace vectorbook;

int main(int argc,  char** argv) {
	absl::ParseCommandLine(argc, argv);

	// ... somewhere inside the `bar(int)` function that is called recursively:
	std::cout << boost::stacktrace::stacktrace();

	return 0;
}
