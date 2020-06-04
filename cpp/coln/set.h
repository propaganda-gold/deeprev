#ifndef coln_set_h
#define coln_set_h 
#include "coln/api.h"

namespace vectorbook {

template <typename T> class SetIterator : public ConstIterator<T> {
public:
  SetIterator(const std::unordered_set<T> *underlying) {
    curr_ = underlying->begin();
    end_ = underlying->end();
  }
  Const<T> current() const override {
		return Unique<T>(&*curr_);
  }
  bool done() { return curr_ == end_; }
  void advance() { ++curr_; }
private:
  typename std::unordered_set<T>::const_iterator curr_;
  typename std::unordered_set<T>::const_iterator end_;
};

template <typename T> class StdSet : public Set<T> {
public:
	StdSet() {}
  StdSet(std::unordered_set<T> &&in) : under_(std::move(in)) {}
  Boolean contains(const T &k) const override {
		auto it = under_.find(k);
		return CopyUnique(it != under_.end());
	}
  Size size() const override { return CopyUnique(under_.size()); }
  Void insert(const T &t) override { under_.insert(t); return Ok(); }
  Void insert(T &&t) override { under_.insert(std::move(t)); return Ok(); }
  Shared<ConstIterator<T>> iterator() const override {
    return Shared<ConstIterator<T>> (new SetIterator<T>(&under_));
  }
	StdSet<T>& operator=(const StdSet<T>& other) {
		StdSet<T> result;
		result.under_ = other.under_;
		return result;
	}
  Void erase(const T &t) override {
		under_.erase(t);
		return Ok();
	}
private:
  std::unordered_set<T> under_;
};

// The default Set, which is backed by a unordered_set.
template <typename T> Unique<Set<T>> MakeSet() {
  return Unique<Set<T>>(new StdSet<T>());
}

} // namespace vectorbook
#endif /* coln_set_h */
