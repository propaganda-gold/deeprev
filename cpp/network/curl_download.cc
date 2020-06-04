#include "network/curl_download.h"

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

namespace {

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

} // namespace

Void CurlDownload(const string &remote_url, const string &local_path) {
	Print_value(local_path);
	ASSERT(!remote_url.empty());
	ASSERT(!local_path.empty());

  FILE *pagefile = fopen(local_path.c_str(), "wb");
  if (!pagefile) {
    RAISE_EXCEPTION("No open.");
	}

	// Initialize: the handle.
  CURL *curl_handle = curl_easy_init();
	// Set: the url.
  curl_easy_setopt(curl_handle, CURLOPT_URL, remote_url.c_str());
	// Set: write function.
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
	// Set: 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

	char *ct = NULL;
	auto info_result = curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_TYPE, &ct);
	if(!info_result) {
		printf("Content-Type: %s\n", ct);
	}
	curl_easy_perform(curl_handle);

  curl_easy_cleanup(curl_handle);
	fclose(pagefile);

  return Ok();
}

} // namespace vectorbook
