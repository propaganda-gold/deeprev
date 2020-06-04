#ifndef util_smtp_mail_h
#define util_smtp_mail_h 
#include "util/includes.h"
#include "util/smtp.pb.h"

namespace vectorbook {


// Loads a static connection.
Void LoadSmtpConnection(const string& file_name);
const SmtpConnection& smtp_connection();

// Uses a pre-loaded static connection.
Void SendEmail(const SmtpRequest& request);

} // namespace vectorbook
#endif /* util_smtp_mail_h */
