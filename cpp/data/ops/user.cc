#include "data/ops/user.h"

#include "data/globals.h"
#include "handlers/api.h"
#include "util/includes.h"
#include "util/uuid.h"

namespace vectorbook {

Option<User> CreateUserOp(const string &email, const string &user_name) {
  User user;
  auto user_id = FreshTimeUUID();
  ASSERT_EXISTS(user_id);
  user.set_id(*user_id);
  user.set_user_name(user_name);
  user.set_email(email);
  ASSERT_SUCCEEDS(data::user_map()->put(user.id(), user));
  ASSERT_SUCCEEDS(data::user_name_map()->put(user_name, user.id()));
  ASSERT_SUCCEEDS(data::email_map()->put(email, user.id()));
  return user;
}

Option<string> TryUserIdFromName(const string &user_name) {
  // Check if the 'user_name' is already taken.
  auto user_id = data::user_name_map()->const_get(user_name);
  ASSERT_OK(user_id);
  if (user_id.has_value()) {
    return *user_id;
  } else {
    return Null();
  }
}

Option<PublicUser> ReduceUserToPublic(const User &user) {
  PublicUser result;
  result.set_user_name(user.user_name());
  result.set_short_description(user.short_description());
  result.set_long_description(user.long_description());
  return result;
}

} // namespace vectorbook
