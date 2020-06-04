#ifndef util_protos_h
#define util_protos_h 
#include "util/includes.h"

namespace vectorbook {

template <typename T> T ParseProto(const std::string &pbtxt) {
  T t;
  google::protobuf::TextFormat::ParseFromString(pbtxt, &t);
  return t;
}

template <typename T> Option<string> ProtoToJsonString(const T &t) {
  string json_string;
  auto message_status =
      google::protobuf::util::MessageToJsonString(t, &json_string);
  ASSERT(message_status.ok());
  return json_string;
}

} // namespace vectorbook
#endif /* util_protos_h */
