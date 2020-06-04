// Unique - constant pointer.. not mutable.. caller doesn't have ownership,
// unless they copy.
// Unique - an moveable object we have owenrship of
// Shared - a copyable object we have owernship of
// Mutable - mutable pointer, we do not have owernship of

// Outside of the collections code, almost no code should use:
//   * unique_ptr (use Unique)
//   * shared_ptr (use Shared)
//
// There should be a rationale listed any time we do not use error-protected
// code.
#ifndef coln_api_h
#define coln_api_h
#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "coln/option.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace vectorbook {

template <typename T> class ConstIterator {
public:
  virtual Const<T> current() const = 0;
  virtual bool done() = 0;
  virtual void advance() = 0;
};

template <typename T> class MutableIterator {
public:
  virtual Mutable<T> current() = 0;
  virtual bool done() = 0;
  virtual void advance() = 0;
};

// We use shared_ptr instead of Unique because a c++ iterator must be copyable.
// http://www.cplusplus.com/reference/iterator
template <typename T> class ConstCppIterator {
public:
  ConstCppIterator<T>() {}
  // 'underyling' must be non-null. We do not use Shared because we will not do
  // error checking.
  ConstCppIterator<T>(const std::shared_ptr<ConstIterator<T>> &underlying)
      : underlying_(underlying) {}
  ConstCppIterator<T> &operator++() {
    underlying_->advance();
    return *this;
  }
  bool operator!=(const ConstCppIterator<T> &iterator) const {
    if (underlying_ && underlying_->done()) {
      return false;
    } else {
      return true;
    }
  }
  Const<T> operator*() const { return underlying_->current(); }

private:
  std::shared_ptr<ConstIterator<T>> underlying_;
};

// We use shared_ptr instead of Unique because a c++ iterator must be copyable.
// http://www.cplusplus.com/reference/iterator
template <typename T> class MutableCppIterator {
public:
  MutableCppIterator<T>() {}
  MutableCppIterator<T>(const std::shared_ptr<MutableIterator<T>> &underlying)
      : underlying_(underlying) {}
  MutableCppIterator<T> &operator++() {
    underlying_->advance();
    return *this;
  }
  bool operator!=(const MutableCppIterator<T> &iterator) const {
    if (underlying_ && underlying_->done()) {
      return false;
    } else {
      return true;
    }
  }
  Mutable<T> operator*() { return underlying_->current(); }

private:
  std::shared_ptr<MutableIterator<T>> underlying_;
};

class Countable {
public:
  virtual Size size() const = 0;
};

template <typename T> class ConstIterable {
public:
  // We return Shared instead of Unique because the cpp iterator must be
  // copyable.
  virtual Shared<ConstIterator<T>> iterator() const = 0;
  ConstCppIterator<T> begin() const {
    return ConstCppIterator<T>(iterator().expose());
  }
  ConstCppIterator<T> end() const { return ConstCppIterator<T>(); }
};

template <typename T> class MutableIterable {
public:
  virtual Shared<MutableIterator<T>> iterator() = 0;

  MutableCppIterator<T> begin() {
    return MutableCppIterator<T>(iterator());
  }
  MutableCppIterator<T> end() { return MutableCppIterator<T>(); }
};

template <typename T>
class ConstCollection : public Countable, public ConstIterable<T> {};
template <typename T>
class MutableCollection : public Countable, public MutableIterable<T> {};

template <typename T> class QueueLike {
public:
  virtual Void append(const T &t) = 0;
  virtual Void append(T &&t) = 0;

	// Returns 1) a value if there is one, 2) Null() if nothing left but no
	// error, or an error otherwise. An implementation can throw NOT_IMPLEMENTED.
  virtual Option<T> pop() = 0;
};

template <typename T>
class Queue : public QueueLike<T>, public ConstCollection<T> {};

template <typename K> class Set : public ConstCollection<K> {
public:
  virtual Boolean contains(const K &k) const = 0;
  virtual Void insert(const K &k) = 0;
  virtual Void insert(K &&k) = 0;
  virtual Void erase(const K &k) = 0;
};

// A sub-class may return "not implemented" for some of these if there is a
// reason, especially 'mutable_get' and 'erase'.
template <typename K, typename V> class MapLike {
public:
  virtual Boolean contains(const K &k) const = 0;
  virtual Const<V> const_get(const K &k) const = 0;
  virtual Void put(const K &k, const V &v) = 0;
  virtual Void put(const K &k, V &&v) = 0;
  virtual Mutable<V> mutable_get(const K &k) = 0;
  virtual Void erase(const K &k) = 0;
};

// A generic map.
// NOTE: Don't make this mutable iterable yet.
template <typename K, typename V>
class Map : public MapLike<K, V>, public ConstCollection<KV<K, V>> {};

template <typename V>
class Seq : public MapLike<size_t, V>,
            public QueueLike<V>,
            public ConstCollection<V> {};

using StringSeq = Seq<std::string>;
using StringMap = Map<std::string, std::string>;

} // namespace vectorbook
#endif /* coln_api_h */
