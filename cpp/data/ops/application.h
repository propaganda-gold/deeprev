#ifndef data_ops_apply_h
#define data_ops_apply_h
#include "data/vectorbook.pb.h"
#include "util/includes.h"

namespace vectorbook {

// Assumes 'email' has been validated.
Option<Application> CreateApplication(const string &email,
                                      const string &textarea);

Boolean MarkApplicationVerified(const string &id);

} // namespace vectorbook
#endif /* data_ops_apply_h */
