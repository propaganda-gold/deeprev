#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/synchronization/mutex.h"
#include "util/includes.h"

using namespace vectorbook;

int main(int argc,  char** argv) {
	absl::ParseCommandLine(argc, argv);

	absl::Mutex mutex;
	// absl::MutexLock lock(&mutex);
	
	size_t i = 0;

	mutex.Lock();
	++i;
	mutex.Unlock();

	return 0;
}
