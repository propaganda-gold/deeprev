#include <iostream>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "util/includes.h"

using namespace vectorbook;

// std::ostream& Info() { return std::cout << "world "; }

int main(int argc,  char** argv) {
	absl::ParseCommandLine(argc, argv);

	Info() << "hi" << endl;
	return 0;
}
