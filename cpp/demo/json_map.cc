#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "util/includes.h"

using namespace std;
using namespace vectorbook;

class JsonBuilder {
public:
	using Map = unordered_map<string, JsonBuilder>;
	JsonBuilder(const unordered_map<string, JsonBuilder>& map) {}
	JsonBuilder(const vector<JsonBuilder>& map) {}
	JsonBuilder(const string& string
};

template <typename V> class JsonMap : public Map<string, V> {
public:
  StdMap() {}
  StdMap(std::unordered_map<K, V> &&in) : under_(std::move(in)) {}
  StdMap(const std::function<V()> &factory_fn) : factory_fn_(factory_fn) {}
  StdMap(const StdMap<K, V> &other) = default;
  StdMap<K, V> &operator=(const StdMap<K, V> &other) = default;
  StdMap(StdMap<K, V> &&other) = default;
  StdMap<K, V> &operator=(StdMap<K, V> &&other) = default;
  Size size() const override { return under_.size(); }
  Boolean contains(const K &k) const { RAISE_EXCEPTION("tbd"); }
  Void put(const K &k, const V &v) override {
    under_[k] = v;
    return Ok();
  }
  Void put(const K &k, V &&v) override {
    under_[k] = std::move(v);
    return Ok();
  }
  Const<V> const_get(const K &k) const override {
    auto ptr = under_.find(k);
    if (ptr != under_.end()) {
      return Const<V>(&(ptr->second));
    } else {
      return Null();
    }
  }
  Mutable<V> mutable_get(const K &k) override {
    auto ptr = under_.find(k);
    if (ptr != under_.end()) {
      return Mutable<V>(&(ptr->second));
    } else {
      if (factory_fn_.has_value()) {
        ASSERT(factory_fn_.has_value());
        under_[k] = (*factory_fn_)();
        auto ptr = under_.find(k);
        return Mutable<V>(&(ptr->second));
      } else {
        return Null();
      }
    }
  }

  Shared<ConstIterator<KV<K, V>>> iterator() const override {
    Shared<ConstIterator<KV<K, V>>> vi(
        new ConstUnorderedMapIterator<K, V>(&under_));
    return vi;
  }

  Void erase(const K &k) override {
    under_.erase(k);
    return Ok();
  }

private:
  std::unordered_map<K, V> under_;
  absl::optional<std::function<V()>> factory_fn_;
};

template <typename JSON>
string BuildJson(const vector<JSON> json_vector) {
	string buffer;
	for (const auto& item : json_vector) {
		buffer += item;
	}
	return buffer;
}

string BuildJson(const unordered_map<string, JsonBuilder> json_map) {
  string buffer;
  for (const auto &kv : json_map) {
    buffer += kv.first;
    buffer += kv.second;
  }
	return buffer;
}

Void Main() {
  unordered_map<string, string> json_map;
  json_map["key.a"] = "value.a";

	JsonMap<JsonSeq<string>
	JsonMap
	StdMap<string, StdSeq<string>> map

	auto result = BuildJson(json_map);
  return Ok();
}

#include "util/main_incl.h"
