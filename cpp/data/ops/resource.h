#ifndef data_ops_link_h
#define data_ops_link_h 
#include "util/includes.h"
#include "data/vectorbook.pb.h"
#include "data/ops/api.pb.h"
#include "util/xml.h"

namespace vectorbook {

// If 'url' isn't in the map, a new entry will be created.
Option<string> ResourceIdFromUrl(const string &url);
Option<Resource> ResourceFromUrl(const string &url);

} // namespace vectorbook
#endif /* data_ops_link_h */
