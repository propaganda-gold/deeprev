#ifndef coln_seq_h
#define coln_seq_h
#include "coln/api.h"

namespace vectorbook {

template <typename T> class ConstStdVectorIterator : public ConstIterator<T> {
public:
  ConstStdVectorIterator(const std::vector<T> *underlying)
      : underlying_(underlying), idx_(0) {}
  Const<T> current() const override { return Const<T>(&underlying_->at(idx_)); }
  bool done() { return idx_ == underlying_->size(); }
  void advance() { ++idx_; }

private:
  const std::vector<T> *underlying_;
  int idx_;
};

template <typename T>
class MutableStdVectorIterator : public MutableIterator<T> {
public:
  MutableStdVectorIterator(std::vector<T> *underlying)
      : underlying_(underlying), idx_(0) {}
  Mutable<T> current() override { return Mutable<T>(&underlying_->at(idx_)); }
  Boolean done() { return idx_ == underlying_->size(); }
  Void advance() {
    ++idx_;
    return Ok();
  }

private:
  std::vector<T> *underlying_;
  int idx_;
};

template <typename T> class StdSeq : public Seq<T> {
public:
  StdSeq() {}
  StdSeq(std::vector<T> &&in) : under_(std::move(in)) {}
	StdSeq(const StdSeq<T>& other) = default;
	StdSeq& operator=(const StdSeq<T>& other) = default;
	StdSeq(StdSeq<T>&& other) = default;
	StdSeq& operator=(StdSeq<T>&& other) = default;
  Const<T> const_get(const size_t &idx) const override {
    return Const<T>(&under_[idx]);
  }
  Mutable<T> mutable_get(const size_t& idx) override {
    return Mutable<T>(&under_[idx]);
  }
  Void put(const size_t& idx, const T &t) override {
    under_[idx] = t;
    return Ok();
  }
  Void put(const size_t& idx, T &&t) override {
    under_[idx] = std::move(t);
    return Ok();
  }
  Size size() const override { return under_.size(); }
  Void append(const T &t) override {
    under_.push_back(t);
    return Ok();
  }
  Void append(T &&t) override {
    under_.push_back(std::move(t));
    return Ok();
  }
	Option<T> pop() override { RAISE_EXCEPTION("Not done."); }
  Shared<ConstIterator<T>> iterator() const override {
    return Shared<ConstIterator<T>>(new ConstStdVectorIterator<T>(&under_));
  }
  Boolean contains(const size_t &k) const override {
		RAISE_EXCEPTION("Not implemented.");
	}
  Void erase(const size_t &k) override {
		RAISE_EXCEPTION("Not implemented.");
	}

private:
  std::vector<T> under_;
};

// This does not implement the copy append.
// TODO(greg) Use c++ traits to eliminate this class?
template <typename T> class UniqueSeq : public Seq<T> {
public:
  UniqueSeq() {}
  UniqueSeq(std::vector<T> &&in) : under_(std::move(in)) {}

  UniqueSeq(const UniqueSeq<T> &) = delete;
  UniqueSeq &operator=(const UniqueSeq<T> &) = delete;

  UniqueSeq(UniqueSeq<T> &&other) { under_ = std::move(other.under_); }
  UniqueSeq &operator=(UniqueSeq<T> &&other) {
    under_ = std::move(other.under_);
  }

  Const<T> const_get(int idx) override { return Const<T>(&under_[idx]); }
  Mutable<T> mutable_get(int idx) override { return Mutable<T>(&under_[idx]); }

  Size size() const override { return under_.size(); }
  Void append(const T &t) override {
    RAISE_EXCEPTION("You cannot call copy-'append' on a UniqueSeq.");
  }
  Void append(T &&t) override {
    under_.push_back(std::move(t));
    return Ok();
  }

  Void put(const size_t &idx, const T &t) {
    RAISE_EXCEPTION("You cannot call copy-'append' on a UniqueSeq.");
  }
  Void put(const size_t &idx, T &&t) {
    under_[idx] = std::move(t);
    return Ok();
  }
  Shared<ConstIterator<T>> iterator() const override {
    return Shared<ConstIterator<T>>(new ConstStdVectorIterator<T>(&under_));
  }

private:
  std::vector<T> under_;
};

// The default Seq, which is backed by a vector.
template <typename T> Unique<Seq<T>> MakeSeq() {
  return Unique<Seq<T>>(new StdSeq<T>());
}

} // namespace vectorbook
#endif /* coln_seq_h */
