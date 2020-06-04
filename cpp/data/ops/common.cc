#include "data/ops/common.h"

#include "util/includes.h"
#include "util/re.h"

namespace vectorbook {

bool IsUrl(const string &token) {
  string http_regex =
      R"(^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.[a-z]{2,5}(:[0-9]{1,5})?(\/.*)?$)";
  vector<string> matches = re::FindAllMatches(token, http_regex);
  return matches.size() > 0;
}

Option<string> LinkKey(const string& url) {
	// TODO(greg) use substr
	ASSERT(url.size() < 1000);
	return url;
}


} // namespace vectorbook
