#include "data/ops/image.h"

#include "data/globals.h"
#include "network/curl_download.h"
#include "util/cassandra_logging.h"
#include "util/includes.h"
#include "util/strings.h"
#include "util/uuid.h"

namespace vectorbook {

namespace {

string LocalImageRoot() { return "/www/images/"; }
Option<string> LocalImageLeafName(const string &id,
                                  const Option<string> &extension) {
  if (extension.has_value()) {
    return StrCat(id, ".", *extension);
  } else {
    return id;
  }
}

Option<string> TryGetExtension(const string &path) {
  std::unordered_set<string> recognized_extensions{"jpg", "png", "svg"};
  auto parts = Split(path, '.');
  auto last_part = parts[parts.size() - 1];

  auto it = recognized_extensions.find(last_part);
  if (it != recognized_extensions.end()) {
    return last_part;
  } else {
    return Null();
  }
}

} // namespace

Option<Image> BuildImage(const string &remote_path) {

  Image result;
  auto id = FreshTimeUUID();
  ASSERT_EXISTS(id);
  result.set_id(*id);

  auto extension_opt = TryGetExtension(remote_path);
  ASSERT_OK(extension_opt);
  if (extension_opt.has_value()) {
    result.set_type(*extension_opt);
  }
  auto local_leaf = LocalImageLeafName(*id, extension_opt);
  ASSERT_EXISTS(local_leaf);
  result.set_file_name(*local_leaf);

  auto local_path = StrCat(LocalImageRoot(), *local_leaf);
  auto download = CurlDownload(remote_path, local_path);
  ASSERT_OK(download);

  ASSERT_SUCCEEDS(
      WriteToCassandra("test1.image", result.id(), result.ShortDebugString()));
  return result;
}

} // namespace vectorbook
