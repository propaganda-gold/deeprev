#include "data/ops/cookietoken.h"

#include "util/includes.h"
#include "util/secure_token.h"
#include "data/globals.h"

namespace vectorbook {

Option<string> TryResolveCookieToken(const string& cookietoken) {
  auto user_id = data::cookietoken_map()->const_get(cookietoken);
  ASSERT_OK(user_id);
	if (user_id.has_value()) {
		return *user_id;
	} else {
		return Null();
	}
}

Option<string> CreateCookieToken(const string& user_id) {
	auto cookietoken = FreshSecureToken(); ASSERT_EXISTS(cookietoken);
	ASSERT_SUCCEEDS(data::cookietoken_map()->put(*cookietoken, user_id));
	return *cookietoken;
}

} // namespace vectorbook
