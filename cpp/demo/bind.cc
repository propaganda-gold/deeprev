#include <functional>   // std::bind

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "util/includes.h"

using namespace vectorbook;
using namespace std::placeholders;    // adds visibility of _1, _2, _3,...


static const char *payload_text[] = {
    "Date: Tue, 19 Nov 2019 21:54:29 +1100\r\n",
    "To: " "greg" "\r\n",
    "From: " "greg2" "\r\n",
    "Subject: SMTP example message\r\n",
    "\r\n", /* empty line to divide headers from body, see RFC5322 */
    "The body of the message starts here.\r\n",
    "\r\n",
    "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
    "Check RFC5322.\r\n",
    NULL};

size_t payload_source2(void *ptr, size_t size, size_t nmemb,
                              void *userp) {
  struct upload_status *upload_status_instance = (struct upload_status *)userp;

  if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
    return 0;
  }

  const char *data = payload_text[upload_status_instance->lines_read];

	assert (data != nullptr);
	size_t len = strlen(data);
	memcpy(ptr, data, len);
	upload_status_instance->lines_read++;

	return len;
}

void Hello(const char* hello[]) {
}

Void Main() {
	Hello(payload_text);

	auto fn2 = std::bind(Hello, payload_text);
	fn2();

	vector<string> vs;
	for(string& s : vs) {
		
	}
	return Ok();
}

#include "util/main_incl.h"
