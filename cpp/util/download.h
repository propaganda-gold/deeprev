#ifndef util_download_h
#define util_download_h 

#include "util/includes.h"

namespace vectorbook {

Option<string> DownloadHttpPage(const string &url);

} // namespace vectorbook
#endif /* util_download_h */
