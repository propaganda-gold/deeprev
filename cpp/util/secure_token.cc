#include "util/secure_token.h"

#include "util/includes.h"
#include "util/random.h"

namespace vectorbook {

Option<string> FreshSecureToken() {
	return Random62String(64);
}

} // namespace vectorbook
