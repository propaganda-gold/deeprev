#ifndef util_check_recaptcha_h
#define util_check_recaptcha_h 
#include "util/includes.h"
#include "curl/curl.h"
#include "util/smtp.pb.h"
#include "util/includes.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

namespace vectorbook {

Option<RecaptchaResponse> CheckRecaptcha(const RecaptchaRequest& request);

Boolean GetRecaptchaSuccess(const string& token);

} // namespace vectorbook
#endif /* util_check_recaptcha_h */
