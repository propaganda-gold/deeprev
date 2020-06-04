#include "util/smtp_mail.h"

#include "curl/curl.h"
#include "util/includes.h"
#include "util/strings.h"

namespace vectorbook {

#define FROM_ADDR                                                              \
  "postmaster@sandboxe9d01c7426f646399fc13e5df22eb237.mailgun.org"
#define TO_ADDR "gfcoppola@gmail.com"

#define FROM_MAIL FROM_ADDR
#define TO_MAIL TO_ADDR

SmtpConnection *mutable_connection() {
  static SmtpConnection *s = new SmtpConnection();
  return s;
}

const SmtpConnection &smtp_connection() { return *mutable_connection(); }

Void LoadSmtpConnection(const string &file_name) {
  // 1) Read 'file_name', to start the Connection.
  auto connection = ReadProto<SmtpConnection>(file_name);
  ASSERT_OK(connection);
  // TODO(greg) Move?
  *mutable_connection() = *connection;

  // 2) Load the password from "password_file". Note: This file is loaded
  // separately so it can be under different access control than the rest of
  // the codebase.
  auto password_contents = ReadFile(mutable_connection()->password_file());
  ASSERT_OK(password_contents);

  auto trimmed = Trim(*password_contents);
  mutable_connection()->set_password(trimmed);
  return Ok();
}

using namespace std::placeholders;

namespace {

struct UploadState {
  int lines_read_;
  vector<string> message_lines_;
};

static const char *payload_text[] = {
    "Date: Tue, 19 Nov 2019 21:54:29 +1100\r\n",
    "To: " TO_MAIL "\r\n",
    "From: " FROM_MAIL "\r\n",
    "Subject: SMTP example message\r\n",
    "\r\n", /* empty line to divide headers from body, see RFC5322 */
    "The body of the message starts here.\r\n",
    "\r\n",
    "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
    "Check RFC5322.\r\n",
    NULL};

size_t TransmitSingleLineFn(void *destination_buffer, size_t size, size_t nmemb,
                            void *provided_user_data) {
  struct UploadState *upload_state = (struct UploadState *)provided_user_data;

  if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
    return 0;
  }

  // const char *outgoing_buffer_data = payload_text[upload_state->lines_read_];
	const char *outgoing_buffer_data = NULL;
	if (upload_state->lines_read_ < upload_state->message_lines_.size()) {
		string curr_line =
				Trim(upload_state->message_lines_[upload_state->lines_read_]);
		curr_line += "\r\n";
		const char *outgoing_buffer_data = curr_line.c_str();
    size_t outgoing_length = strlen(outgoing_buffer_data);
    // void * memcpy ( void * destination, const void * source, size_t num )
    memcpy(destination_buffer, outgoing_buffer_data, outgoing_length);
    upload_state->lines_read_++;
    return outgoing_length;
	}

  return 0;
}

class CurlWrapper {
private:
  vector<string> message_lines_;

public:
  CurlWrapper(vector<string> &&message_lines)
      : message_lines_(std::move(message_lines)) {}
  static CurlWrapper Create(const SmtpRequest &request) {
    auto payload_tmpl = R"(Date: $date
To: $to_address
From: $from_address
Broadcast-ID: $message_id
Subject: $subject

$body
)";
    auto payload_cppstr = Replace(
        payload_tmpl, {
                          {"$date", request.date()},
                          {"$to_address", request.to_address()},
                          {"$from_address", smtp_connection().from_address()},
                          {"$message_id", request.message_id()},
                          {"$subject", request.subject()},
                          {"$body", request.body()},
                      });
    vector<string> lines = Split(payload_cppstr, "\n");
		Output_each(lines);
    return CurlWrapper(std::move(lines));
  }

  Void SendEmail(const SmtpRequest &request) {
		Output_debug(request);
    CURL *curl;
    struct curl_slist *recipients = NULL;
    struct UploadState upload_state;

    upload_state.lines_read_ = 0;
    upload_state.message_lines_ = message_lines_;

    curl = curl_easy_init();
    if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL,
                       smtp_connection().smtp_address().c_str());
      //      curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
      curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);

      curl_easy_setopt(curl, CURLOPT_USERNAME,
                       smtp_connection().user_name().c_str());
      curl_easy_setopt(curl, CURLOPT_PASSWORD,
                       smtp_connection().password().c_str());

      curl_easy_setopt(curl, CURLOPT_MAIL_FROM,
                       smtp_connection().from_address().c_str());
      recipients = curl_slist_append(recipients, request.to_address().c_str());
      curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
      curl_easy_setopt(curl, CURLOPT_READFUNCTION, TransmitSingleLineFn);
      curl_easy_setopt(curl, CURLOPT_READDATA, &upload_state);
      curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
      curl_easy_setopt(curl, CURLOPT_PORT, 587L);

      //
      // Perform.
      const CURLcode curl_result = curl_easy_perform(curl);
      ASSERT(curl_result == CURLE_OK);

      curl_slist_free_all(recipients);
      curl_easy_cleanup(curl);
    }

    return Ok();
  }
};

} // namespace

Void SendEmail(const SmtpRequest &request) {
  auto wrapper = CurlWrapper::Create(request);
  ASSERT_SUCCEEDS(wrapper.SendEmail(request));
  return Ok();
}

} // namespace vectorbook
