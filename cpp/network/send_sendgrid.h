#ifndef network_send_sendgrid_h
#define network_send_sendgrid_h 
#include "util/includes.h"
#include "network/requests.pb.h"

namespace vectorbook {

Void LoadSendgridSecretFile(const string& file_path);

Void SendSendgrid(const SendgridRequest& request);

} // namespace vectorbook
#endif /* network_send_sendgrid_h */
