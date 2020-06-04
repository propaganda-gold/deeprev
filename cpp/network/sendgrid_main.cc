#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "network/send_sendgrid.h"
#include "util/includes.h"

using namespace vectorbook;

ABSL_FLAG(std::string, from, "", "");
ABSL_FLAG(std::string, to, "", "");
ABSL_FLAG(std::string, secret, "", "");

Void Main() {
  auto from = absl::GetFlag(FLAGS_from);
  ASSERT(!from.empty());
  auto to = absl::GetFlag(FLAGS_to);
  ASSERT(!to.empty());
  auto secret = absl::GetFlag(FLAGS_secret);
  ASSERT(!secret.empty());


  // Make a request.
  SendgridRequest request;
  request.set_to_address(to);
  request.set_from_address(from);
  request.set_from_name("c++ tester");
  request.set_subject("c++ sendgrid test");
  request.set_body("<html><body><strong>// TODO(greg) migrate to node.js</strong></html>");
	request.set_secret(secret);

  // Send the request.
  auto sendgrid_result = SendSendgrid(request);
  ASSERT_OK(sendgrid_result);
  return Ok();
}

#include "util/main_incl.h"
