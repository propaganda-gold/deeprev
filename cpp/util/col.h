#ifndef util_col_h
#define util_col_h
#include "absl/types/optional.h"
#include "google/protobuf/map.h"
#include "util/includes.h"
#include <unordered_map>

namespace vectorbook {

template <typename K, typename V>
Unique<V> MapGet(const std::unordered_map<K, V> &map, const K &key) {
  auto it = map.find(key);
  if (it != map.end()) {
    return new V(it->second);
  } else {
    return Null();
  }
}

template <typename K, typename V>
Option<V> ProtoGet(const google::protobuf::Map<K, V> &map, const K &key) {
  auto it = map.find(key);
  if (it != map.end()) {
    return Trim(it->second);
  } else {
    return Null();
  }
}

template <typename K, typename V>
Mutable<V> ProtoMutableGet(google::protobuf::Map<K, V> *map, const K &key) {
  auto it = map->find(key);
  if (it != map->end()) {
    return new V(it->second);
  } else {
    return Null();
  }
}

template <typename T> std::unordered_set<T> ToSet(const std::vector<T> &col) {
  std::unordered_set<T> result;
  for (const T &t : col) {
    result.insert(t);
  }
  return result;
}

template <typename T>
std::vector<T> ToVector(const std::unordered_set<T> &col) {
  std::vector<T> result;
  for (const T &t : col) {
    result.push_back(t);
  }
  return result;
}

template <typename T> void SortVector(std::vector<T> *col) {
  std::sort(col->begin(), col->end());
}

template <typename T>
std::vector<T> CopySlice(const std::vector<T> &coln, unsigned int start,
                         unsigned int end) {
  std::vector<T> buffer;
  for (unsigned int i = start; i < end; ++i) {
    buffer.push_back(coln[i]);
  }
  return buffer;
}

template <typename T>
void AppendVector(const std::vector<T> &coln, std::vector<T> *collector) {
  for (const T &t : coln) {
    collector->push_back(t);
  }
}

template <typename T>
std::vector<T> Flatten(const std::vector<std::vector<T>> &vectors) {
  std::vector<T> buffer;
  for (const std::vector<T> &vector_ : vectors) {
    for (const T &t : vector_) {
      buffer.push_back(t);
    }
  }
  return buffer;
}

template <typename T>
unsigned int FindIndex(const std::vector<T> &coln, const T &t) {
  for (unsigned int i = 0; i < coln.size(); ++i) {
    if (coln[i] == t) {
      return i;
    }
  }
  return -1;
}

} // namespace vectorbook
#endif /* util_col_h */
