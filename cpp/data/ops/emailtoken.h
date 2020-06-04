#ifndef data_ops_emailtoken_h
#define data_ops_emailtoken_h 
#include "util/includes.h"

namespace vectorbook {

// Returns the string-valued email address associated with 'emailtoken', or
// Null() if this does not exist.
Option<string> TryResolveEmailToken(const string& emailtoken);

Option<string> CreateEmailToken(const string& email);

Void EraseEmailToken(const string& emailtoken);

// Note: 'email' should be validated on the javascript end. If someone sends us
// a request outside of that, Sendgrid will detect a bad email address.
Void CreateTokenAndSendEmail(const string& email);


} // namespace vectorbook
#endif /* data_ops_emailtoken_h */
