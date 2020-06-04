#ifndef data_ops_applytoken_h
#define data_ops_applytoken_h 
#include "util/includes.h"

namespace vectorbook {

Option<string> TryResolveApplyToken(const string &applytoken) ;

Option<string> CreateApplyToken(const string &email) ;

Void EraseApplyToken(const string &applytoken) ;

} // namespace vectorbook
#endif /* data_ops_applytoken_h */
