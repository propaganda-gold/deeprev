#ifndef data_ops_common_h
#define data_ops_common_h 
#include "util/includes.h"

namespace vectorbook {

bool IsUrl(const string &token);

Option<string> LinkKey(const string& url);

} // namespace vectorbook
#endif /* data_ops_common_h */
