#ifndef data_ops_image_h
#define data_ops_image_h 
#include "util/includes.h"
#include "data/vectorbook.pb.h"

namespace vectorbook {

Option<Image> BuildImage(const string& remote_path);

} // namespace vectorbook
#endif /* data_ops_image_h */
