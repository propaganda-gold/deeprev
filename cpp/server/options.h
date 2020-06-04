#ifndef server_options_h
#define server_options_h 
#include "util/includes.h"
#include "server/options.pb.h"

namespace vectorbook {
namespace statics {

// 'file_path' must point to a pbtxt of type 'OptionBundle'.
Void LoadRestOptions(const string& file_path);

const RestOptions& rest_options();

// 'file_path' must point to a pbtxt of type 'OptionBundle'.
Void LoadSecrets(const string& file_path);
const Secrets& secrets();

} // namespace statics
} // namespace vectorbook
#endif /* server_options_h */
