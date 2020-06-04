#ifndef data_converter_h
#define data_converter_h
#include "util/includes.h"

namespace vectorbook {
namespace data {

template <typename T> class Serializer {
public:
  virtual Unique<string> Serialize(const T &t) const = 0;
  virtual Unique<T> Parse(const string &t) const = 0;
};

namespace impl {
template <typename T> class DebugSerializer : public Serializer<T> {
public:
  Unique<string> Serialize(const T &t) const override { return MoveUnique(t.DebugString()); }
  Unique<T> Parse(const string &t) const override {
    T result;
    ASSERT(TextFormat::ParseFromString(t, &result));
    return MoveUnique(std::move(result));
  }
};
} // namespace impl

template <typename T> Unique<Serializer<T>> MakeDebugSerializer() {
  return Unique<Serializer<T>>(new impl::DebugSerializer<T>());
}

} // namespace data
} // namespace vectorbook
#endif /* data_converter_h */
