#include "util/includes.h"

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

namespace vectorbook {

time_t CurrentTimeInfo() {
	return time(NULL);
}

}  // namespace vectorbook
