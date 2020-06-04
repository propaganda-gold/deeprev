#ifndef coln_option_h
#define coln_option_h
#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "absl/types/optional.h"
#include "util/logging.h"
#include "util/stacktrace.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace vectorbook {

class Error {
public:
  Error() {}
  //  Error(const std::string &message) : message_(message) {}
  Error(const std::string &file_name, int line, const std::string &message)
      : message_(message) {
    add_location(file_name, line);
  }

  const std::string &message() const { return message_; }

  std::string StackTrace() const {
    std::string buffer = message_;
    for (size_t i = 0; i < file_list_.size(); ++i) {
      buffer += absl::StrCat("\n>>>>", file_list_[i], ":", line_list_[i]);
    }
    return buffer;
  }

  Error &add_location(const std::string &file, int line) {
    file_list_.push_back(file);
    line_list_.push_back(line);
    return *this;
  }
  bool is_error() { return !message_.empty(); }

private:
  std::string message_;
  std::vector<std::string> file_list_;
  std::vector<int> line_list_;
};

// Such a class can be "checked" for
//   1) an error
//   2) a "null" value
// We emphasize: these are two separate things.
//
// An 'error' indicates that something went WRONG. This should be the result
// of:
//   1) a bug, that should be fixed, an ERROR
//   2) an unfortunate run-time factor, that could not be overcome, but does
//   NOT indicate a bug that must be fixed, a WARNING
//
// In terms of handling:
//   1) an error is indicated by wrapping an Error object. This works the same
//   for each sub-class, so is implemented in the base class.
//
// Mutables: This class uses the "mutable" keyword to cache values while still
// declaring "const-style" methods const. If, e.g., "has_error()" were not
// marked "const" in a const context.
class Checkable {
public:
  ~Checkable() {
    // TODO(greg) Put a check here.
  }
  Checkable() {}
  explicit Checkable(const Error &error) : error_(new Error(error)) {}

  virtual bool has_value() const = 0;

  Checkable(Checkable &&other) = default;
  Checkable &operator=(Checkable &&other) = default;
  Checkable(const Checkable &other) = default;
  Checkable &operator=(const Checkable &other) = default;

  // Should not be called unless 'has_error' is true.
  const Error &error() const {
    assert(error_ != nullptr);
    return *error_;
  }

  bool has_error() const {
    error_checked_at_all_ = true;
    const bool is_error = error_ != nullptr && error_->is_error();
    if (!is_error) {
      error_checked_successfully_ = true;
    }
    return is_error;
  }
  bool ok() const { return !has_error(); }

  bool error_checked_successfully() const {
    return error_checked_successfully_;
  }
  bool value_checked_successfully() const {
    return value_checked_successfully_;
  }
  void set_value_checked_successfully() { value_checked_successfully_ = true; }

  std::string status() const {
    std::string error_string = has_error() ? error().StackTrace() : "OK";
    std::string has_value_string = has_value() ? "true" : "false";
    return absl::StrCat("Checkable[status=", error_string,
                        ",has_value=", has_value_string, "]");
  }

protected:
private:
  // Error must be checked in order to dereference.
  // TODO(greg) create a "MUST USE" to ensure voids are also checked.
  mutable bool error_checked_at_all_ = false;
  mutable bool error_checked_successfully_ = false;
  mutable bool value_checked_successfully_ = false;
  std::unique_ptr<Error> error_;
};

class Null {};

class Void : public Checkable {
public:
  Void() {}
  // IMPLICIT CONVERSIONS: Yes.
  Void(const Null &null) {}
  // IMPLICIT CONVERSIONS: Yes.
  Void(const Error &error) : Checkable(error) {}

  Void(Void &&other) {}
  Void(const Void &other) {}
  Void &operator=(Void &&other) { throw "unhandled"; }
  Void &operator=(const Void &other) { throw "unhandled"; }

  // A Void never has a value.
  bool has_value() const override { return false; }
};

// Creates a "no-error" Error.
Error Ok();

// Unique wraps a "unique_ptr" and an error code.
//
// USAGE:
//   For a copyable, concrete type, use Option instead.
//   Unique is for:
//   1) Primarily, Unique is meant for values of T that are **abstract**, and
//   thus cannot be stored as POD.
//   2) Also, Unique can be used if T is not copyable, but switching blaze to
//   use c++11 will be another way to fix that.
template <typename T> class Unique : public Checkable {
public:
  // Constructors from other objects.
  Unique() {}
  Unique(const Null &null) {}
  Unique(T *p) : under_(p) {}
  Unique(std::unique_ptr<T> &&p) { under_ = std::move(p); }
  Unique(const Error &error) : Checkable(error) {}

  // Delete copy constructors.
  Unique(const Unique<T> &p) = delete;
  Unique &operator=(const Unique<T> &p) = delete;

  // Move constructors.
  Unique(Unique<T> &&p) { under_ = std::move(p.under_); }
  Unique &operator=(Unique<T> &&p) {
    under_ = std::move(p.under_);
    return *this;
  }

  // Dereference.
  T &operator*() {
    assert(has_value());
    return *under_;
  }
  T *operator->() { return &operator*(); }
  const T &operator*() const { return *under_; }
  const T *operator->() const { return &operator*(); }

  T &expose() { return *under_; }
  T *release() { return under_.release(); }
  T *get() { return under_.get(); }
  void reset(T *p) { under_.reset(p); }
  bool has_value() const override { return under_ != nullptr; }

private:
  std::unique_ptr<T> under_;
};

// 'Option' is for concrete (non-virtual) values of T.
template <typename T> class Option : public Checkable {
public:
  // Constructors from other objects.
  Option() {}
  Option(const Null &null) {}
  Option(const T &p) : under_(p) {}
  Option(T &&p) : under_(std::move(p)) {}
  Option(const Error &error) : Checkable(error) {}

  Option(const Option<T> &p) = default;
  Option &operator=(const Option<T> &p) = default;
  Option(Option<T> &&p) = default;
  Option &operator=(Option<T> &&p) = default;

  // Dereference.
  T &operator*() {
    assert(has_value());
    return *under_;
  }
  T *operator->() { return &operator*(); }
  const T &operator*() const { return *under_; }
  const T *operator->() const { return &operator*(); }

  bool has_value() const override { return under_.has_value(); }

private:
  absl::optional<T> under_;
};

// Scheme
//            Ownership    Mutable    Copyable
// Unique     Yes          Yes        No
// Shared     Yes          Yes        Yes
// Const      No           No         Yes
// Mutable    No           Yes        Yes

// OVERVIEW: A checkable "const reference".
// OWNERSHIP: Conceptually, Unique signifies NO TRANSFER in ownership. However,
// for interfaces where either a const-reference or a copied-object may either
// be returned
// STATES:
//   Either:
//   1) error
//   2) null
//   3) contains payload
// BACKING:
//   For payload either:
//   1) pointer
//   2) unique_ptr
template <typename T> class Const : public Checkable {
public:
  // Constructors from other objects.
  Const() {}
  Const(const Null &null) {}
  Const(const T *p) : raw_ptr_(p) {} // OWNERSHIP: No ownership transfer.
  Const(std::unique_ptr<T> &&p) { under_ = std::move(p); }
  Const(Unique<T> &&p) { under_.reset(p.release()); }
  Const(const Error &error) : Checkable(error) {}

  // Delete copy constructors.
  Const(const Const<T> &p) = delete;
  Const &operator=(const Const<T> &p) = delete;

  // Move constructors.
  Const(Const<T> &&p) {
    raw_ptr_ = p.raw_ptr_;
    under_ = std::move(p.under_);
  }
  Const &operator=(Const<T> &&p) {
    raw_ptr_ = p.raw_ptr_;
    under_ = std::move(p.under_);
    return *this;
  }

  // Dereference.
  const T &operator*() const {
    if (raw_ptr_) {
      return *raw_ptr_;
    } else if (under_) {
      return *under_;
    } else {
      // assert(error_checked_successfully());
      if (!has_value()) {
        // Output_() << StackTrace() << endl;
      }
      return *under_;
    }
  }

  // This should only be called if the underlying data is locally stored.
  T &expose() { return *under_; }

  const T *operator->() const { return &operator*(); }
  bool has_value() const override {
    // assert(error_checked_successfully());
    return raw_ptr_ || under_;
  }

private:
  const T *raw_ptr_ = nullptr;
  std::unique_ptr<T> under_;
};

template <typename T> Unique<T> MoveUnique(T &&t) {
  return Unique<T>(new T(std::move(t)));
}

template <typename T> Unique<T> CopyUnique(const T &t) {
  return Unique<T>(new T(t));
}

template <typename T> class Shared : public Checkable {
public:
  Shared() {}
  // OWNERSHIP: Takes ownership.
  Shared(T *t) { t_.reset(t); }
  Shared(const T &t) { t_.reset(new T(t)); }
  Shared(const std::shared_ptr<T> &t) { t_ = t; }
  Shared(const Error &error) : Checkable(error) {}
  Shared(const Shared<T> &other) : t_(other.t_) {}
  Shared<T> &operator=(const Shared<T> &other) { t_ = other.t_; }

  T &operator*() { return *t_; }
  T *operator->() { return &operator*(); }
  const T &operator*() const { return *t_; }
  const T *operator->() const { return &operator*(); }

  std::shared_ptr<T> expose() const { return t_; }
  bool has_value() const override { return t_ != nullptr; }

private:
  std::shared_ptr<T> t_;
};

// Mutable and Copyable.
// STATES:
//   1) error
//   2) null
//   3) payload
template <typename T> class Mutable : public Checkable {
public:
  using WriteBackFn = std::function<Void(const T &)>;
  Mutable() {}
  Mutable(const Null &null) {}
  Mutable(T *p) : raw_ptr_(p) {}

  // Takes ownership!
  Mutable(const WriteBackFn &write_fn, T *p) : raw_ptr_(p), write_fn_(write_fn) {}
  Mutable(const WriteBackFn &write_fn, std::unique_ptr<T> &&p) {
    local_copy_ = std::move(p);
    raw_ptr_ = local_copy_.get();
    write_fn_ = write_fn;
  }
  Mutable(const Error &error) : Checkable(error) {}

  Mutable(Mutable &&other) {
    local_copy_ = std::move(other.local_copy_);
    raw_ptr_ = local_copy_.get();
    write_fn_ = std::move(other.write_fn_);
  }
  Mutable &operator=(Mutable &&other) {
    local_copy_ = std::move(other.local_copy_);
    raw_ptr_ = local_copy_.get();
    write_fn_ = std::move(other.write_fn_);
    return *this;
  }
  Mutable(const Mutable &other) = delete;
  Mutable &operator=(const Mutable &other) = delete;

  T &operator*() { return *raw_ptr_; }
  T *operator->() { return raw_ptr_; }
  bool has_value() const override { return raw_ptr_ != nullptr; }

  Void write() {
    if (local_copy_ != nullptr) {
      auto status = write_fn_(*local_copy_);
      if (!status.ok()) {
        return status;
      }
      return status;
    } else {
      return Ok();
    }
  }

private:
  T *raw_ptr_ = nullptr;

  // For remote maps, we can store the value here, and then write it back when
  // the user calls "write".
  // Note: This does not need to be used.
  std::unique_ptr<T> local_copy_;
  // Note: This is used iff local_copy_ is.
  WriteBackFn write_fn_;
};

template <typename K, typename V> class KV {
public:
  KV() {}
  KV(const K &key, const V &value) : key_(key), value_(value) {}
  KV(const std::pair<const K, V> *under) : under_(under) {}

  const K &key() const {
    if (under_) {
      return under_->first;
    } else {
      return key_;
    }
  }
  const V &value() const {
    if (under_) {
      return under_->second;
    } else {
      return value_;
    }
  }

private:
  const std::pair<const K, V> *under_ = nullptr;
  K key_;
  V value_;
};

// Short-hands.
using Boolean = Option<bool>;
using Size = Option<size_t>;

// Error checking.
std::unique_ptr<Error> CheckImpl(const std::string &file, int line,
                                 Checkable &checkable);

std::unique_ptr<Error> CheckExistsImpl(const std::string &expr_name,
                                       const std::string &file, int line,
                                       Checkable &checkable);

#define ASSERT_OK(expr)                                                               \
  {                                                                            \
    auto check_impl = CheckImpl(__FILE__, __LINE__, (expr));                   \
    if (check_impl != nullptr) {                                               \
      return *check_impl;                                                      \
    }                                                                          \
  }

#define ASSERT_SUCCEEDS(expr)                                                              \
  {                                                                            \
    auto status = (expr);                                                      \
    if (!status.ok()) {                                                        \
      auto error = status.error(); \
			return error.add_location(__FILE__, __LINE__);                                                   \
    }                                                                          \
  }

#define RAISE_EXCEPTION(expr)                                                            \
  { return Error(__FILE__, __LINE__, (expr)); }

#define ASSERT(expr)                                                           \
  {                                                                            \
    if (!(expr)) {                                                             \
      return Error(__FILE__, __LINE__, #expr);                                 \
    }                                                                          \
  }

#define DEBUG_ASSERT(expr)                                                           \
  {                                                                            \
    if (!(expr)) {                                                             \
      return Error(__FILE__, __LINE__, #expr);                                 \
    }                                                                          \
  }

#define ASSERT_EXISTS(expr)                                                           \
  {                                                                            \
    auto check_impl = CheckExistsImpl(#expr, __FILE__, __LINE__, (expr));      \
    if (check_impl != nullptr) {                                               \
      return *check_impl;                                                      \
    }                                                                          \
  }

std::string PrettyStringFromError(Checkable &checkable);
void PrettyPrintError(Checkable &checkable);

} // namespace vectorbook
#endif /* coln_option_h */
