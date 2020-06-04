#ifndef data_ops_post_h
#define data_ops_post_h 
#include "util/includes.h"
#include "data/vectorbook.pb.h"
#include "data/ops/api.pb.h"

namespace vectorbook {

Option<Broadcast> BuildBroadcast(const Broadcast::BasicData& basic_data);

} // namespace vectorbook
#endif /* data_ops_post_h */
