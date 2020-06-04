#include "data/ops/applytoken.h"

#include "data/globals.h"
#include "util/includes.h"
#include "util/secure_token.h"

namespace vectorbook {

Option<string> TryResolveApplyToken(const string &applytoken) {
  auto email = data::applytoken_map()->const_get(applytoken);
  ASSERT_OK(email);
  if (email.has_value()) {
    return *email;
  } else {
    return Null();
  }
}

Option<string> CreateApplyToken(const string &id) {
	auto applytoken = FreshSecureToken(); ASSERT_EXISTS(applytoken);
  auto status = data::applytoken_map()->put(*applytoken, id);
  ASSERT_OK(status);
  return *applytoken;
}

Void EraseApplyToken(const string &applytoken) {
  auto status = data::applytoken_map()->erase(applytoken);
  ASSERT_OK(status);
  return Ok();
}

} // namespace vectorbook
