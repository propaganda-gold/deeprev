#ifndef coln_iterable_h
#define coln_iterable_h
#include "coln/api.h"

namespace vectorbook {

template <typename T> class EmptyConstIteratorImpl : public ConstIterator<T> {
  const T &current() const {
    throw "EmptyConstIterator: 'current' called on empty iterator.";
  }
  bool done() { return true; }
  void advance() {
    throw "EmptyConstIterator: 'advance' called on empty iterator.";
  }
};

template <typename T> std::unique_ptr<ConstIterator<T>> EmptyConstIterator() {
  return std::unique_ptr<ConstIterator<T>>(new EmptyConstIteratorImpl<T>());
}

template <typename T> class EmptyConstIterableImpl : public ConstIterable<T> {
  std::unique_ptr<ConstIterator<T>> iterator() const override {
    return EmptyConstIterator<T>();
  }
};

template <typename T> std::unique_ptr<ConstIterable<T>> EmptyConstIterable() {
  return std::unique_ptr<ConstIterable<T>>(new EmptyConstIterableImpl<T>());
}

} // namespace vectorbook
#endif /* coln_iterable_h */
