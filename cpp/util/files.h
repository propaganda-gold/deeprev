#ifndef util_files_h
#define util_files_h
#include "coln/option.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/stubs/status.h"
#include "google/protobuf/util/json_util.h"
#include "util/strings.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace vectorbook {

bool FileExists(const std::string &file_name);
Void WriteFile(const std::string &file_name, const std::string &contents);
Void WriteLines(const std::string &file_name,
                const std::vector<std::string> &contents);

Option<std::string> ReadFile(const std::string &file_name);
Option<std::vector<std::string>> ReadLines(const std::string &file_name);
Option<std::string> ReadSecretFromFile(const std::string &file_name);

template <typename T>
Void WriteProtos(const std::string &file_name, const std::vector<T> &protos) {
  std::string buffer;
  for (const T &proto : protos) {
    buffer += proto.ShortDebugString() + '\n';
  }
  ASSERT_SUCCEEDS(WriteFile(file_name, buffer));
	return Ok();
}

template <typename T> Option<T> ReadProto(const std::string &file_name) {
  auto buffer = ReadFile(file_name); ASSERT_OK(buffer);
	T proto;
  ASSERT(google::protobuf::TextFormat::ParseFromString(*buffer, &proto));
  return proto;
}

template <typename T>
Option<std::vector<T>> ReadProtos(const std::string &file_name) {
  auto buffer = ReadFile(file_name); ASSERT_OK(buffer);
  std::vector<std::string> lines = Split(*buffer, "\n");
	std::vector<T> result;
  for (const std::string &line : lines) {
    std::string stripped = Trim(line);
    if (stripped.empty()) {
      continue;
    }
    T t;
    google::protobuf::TextFormat::ParseFromString(line, &t);
    result.push_back(std::move(t));
  }
	return result;
}

//template <typename T> Option<string> ProtoToJsonString(const T &t) {
//  string json_string;
//  auto message_status =
//      google::protobuf::util::MessageToJsonString(t, &json_string);
//  ASSERT(message_status.ok());
//  return json_string;
//}

} // namespace vectorbook
#endif /* util_files_h */
