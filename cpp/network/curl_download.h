#ifndef network_curl_download_h
#define network_curl_download_h 
#include "util/includes.h"

namespace vectorbook {

namespace curl {
Void Initialize();
Void CleanUp();
}  // namespace curl

Void CurlDownload(const string& remote_url, const string& local_path);

} // namespace vectorbook
#endif /* network_curl_download_h */
