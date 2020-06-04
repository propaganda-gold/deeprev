#ifndef data_ops_user_h
#define data_ops_user_h
#include "data/ops/api.pb.h"
#include "data/vectorbook.pb.h"
#include "handlers/http.pb.h"
#include "util/includes.h"

namespace vectorbook {

// This function assumes:
//  1. 'email' has been verified
// This function will check:
//  2. 'user_name' is free
Option<User> CreateUserOp(const string &email,
                          const string &user_name);


Option<string> TryUserIdFromName(const string& user_name);

Option<PublicUser> ReduceUserToPublic(const User& user);

} // namespace vectorbook
#endif /* data_ops_user_h */
