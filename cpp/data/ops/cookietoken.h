#ifndef data_ops_cookietoken_h
#define data_ops_cookietoken_h 
#include "util/includes.h"

namespace vectorbook {

// Tries to find the 'user_id' for 'cookietoken', otherwise returns Null().
Option<string> TryResolveCookieToken(const string& cookietoken) ;

// Creates and sets a cookietoken 'user_id'.
Option<string> CreateCookieToken(const string& user_id);

} // namespace vectorbook
#endif /* data_ops_cookietoken_h */
