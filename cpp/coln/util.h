#ifndef coln_util_h
#define coln_util_h
#include "coln/api.h"
#include "coln/seq.h"
#include "util/random.h"
#include <iostream>

namespace vectorbook {

template <typename T> Unique<Seq<T>> ToSeq(const ConstIterable<T> &iterable) {
  Unique<Seq<T>> buffer(new StdSeq<T>());
  for (auto t : iterable) {
		ASSERT_OK(t);
    buffer->append(*t);
  }
  return buffer;
}

template <typename T> StdSeq<T> MoveToSeq(const ConstIterable<T> &iterable) {
  StdSeq<T> buffer;
  for (auto t : iterable) {
    buffer.append(std::move(*t));
  }
  return buffer;
}

// Reverses a constant sequence.
template <typename T> Unique<Seq<T>> ReverseSeq(const Seq<T> &seq) {
  Unique<Seq<T>> buffer(new StdSeq<T>());
  auto size = seq.size();
  ASSERT_OK(size);
  for (size_t i = 0; i < *size; ++i) {
    auto pointer = seq.const_get(*size - i - 1);
    ASSERT_OK(pointer);
    buffer->append(*pointer);
  }
  return buffer;
}

// template <typename T> StdSeq<T> MoveReverseSeq(Seq<T> *seq) {
//  StdSeq<T> buffer;
//  for (size_t i = 0; i < *seq->size(); ++i) {
//    auto t = seq->at(seq->size() - i - 1);
//    buffer.append(std::move(t));
//  }
//  return buffer;
//}

template <typename T> const T &RandomFromSeq(const Seq<T> &seq) {
  size_t idx = RandomInt(seq.size());
  return seq.at(idx);
}

template <typename T>
Unique<T> RandomFromIterable(const ConstCollection<T> &itbl) {
  auto size = itbl.size();
  ASSERT_OK(size);
  if (*size == 0) {
    RAISE_EXCEPTION("itbl.size() == 0");
  }
  size_t idx = RandomInt(*size);
  size_t i = 0;
  for (auto opt : itbl) {
    if (i == idx) {
      return new T(*opt);
    }
    ++i;
  }
  RAISE_EXCEPTION("Should not reach here.");
}

template <typename T>
Void AppendAll(Seq<T>* into, const std::vector<T>& from) {
	for (const auto& f : from) {
		auto status = into->append(f); ASSERT_OK(status);
	}
	return Ok();
}

} // namespace vectorbook
#endif /* coln_util_h */
