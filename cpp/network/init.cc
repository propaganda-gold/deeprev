#include "network/init.h"

#include "util/includes.h"
#include <curl/curl.h>

namespace vectorbook {
namespace curl {

Void Initialize() {
  const CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
  if (res == CURLE_OK) {
		return Ok();
  } else {
		RAISE_EXCEPTION(StrCat("Error connecting to curl: ", res));
	}
}

Void CleanUp() {
  curl_global_cleanup(); // is void
	// See : https://curl.haxx.se/libcurl/c/curl_global_cleanup.html
	return Ok();
}

} // namespace curl
} // namespace vectorbook
