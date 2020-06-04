#include "server/options.h"

#include "util/includes.h"

namespace vectorbook {
namespace statics {

namespace {

RestOptions *mutable_options_spec() {
  static RestOptions *single = new RestOptions();
  return single;
}

} // namespace

Void LoadRestOptions(const string &file_path) {
  auto spec = ReadProto<RestOptions>(file_path);
  ASSERT_OK(spec);
  *mutable_options_spec() = *spec;
  return Ok();
}

const RestOptions &rest_options() { return *mutable_options_spec(); }

namespace {

Secrets *mutable_secrets() {
  static Secrets *single = new Secrets();
  return single;
}

} // namespace

Void LoadSecrets(const string &file_path) {
  auto spec = ReadProto<Secrets>(file_path);
  ASSERT_OK(spec);
  *mutable_secrets() = *spec;
  return Ok();
}

const Secrets &secrets() { return *mutable_secrets(); }

} // namespace statics
} // namespace vectorbook
