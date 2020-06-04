#ifndef data_api_h
#define data_api_h
#include "util/includes.h"

namespace vectorbook {

// A "generic" key abstracts over how the key is really structured, but can
// always convert to a string.
class GenericKey {
public:
  virtual string ToString() const = 0;
};

namespace keys {

class StringKey : public GenericKey {
public:
  StringKey(const string &under) : under_(under) {}
  string ToString() const override { return under_; }

private:
  string under_;
};

StringKey FromString(const string &key);

} // namespace keys
} // namespace vectorbook
#endif /* data_api_h */
