// Demos how to call out to Google's "recaptcha v3", using libcurl.

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include "curl/curl.h"
#include "demo/api.pb.h"
#include "util/includes.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

// "secret" to identify this site for recaptcha.
ABSL_FLAG(std::string, secret, "", "");

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if (ptr == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);
  std::string secret = absl::GetFlag(FLAGS_secret);

  if (secret.empty()) {
    std::cout << "some input is empty: secret [" << secret << "]\n";
    return 1;
  }
  CURL *curl;
  CURLcode res;

  struct MemoryStruct chunk;
  chunk.memory =
      (char *)malloc(1); /* will be grown as needed by the realloc above */
  chunk.size = 0;        /* no data at this point */

  /* In windows, this will init the winsock stuff */
  res = curl_global_init(CURL_GLOBAL_DEFAULT);
  /* Check for errors */
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed: %s\n", curl_easy_strerror(res));
    return 1;
  }

  /* get a curl handle */
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL,
                     "https://api.sendgrid.com/v3/mail/send");

    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    std::string post_data;
    post_data += R"({"personalizations": [{"to": [{"email": "gfcoppola@gmail.com"}]}],"from": {"email": "welcome@deeprevelations.com"},"subject": "Verify Email to Get Priority Number","content": [{"type": "text/plain", "value": "Please verify your email using the link https://deeprevelations.com/acknowledge_registration?email_key=gfcoppola%40gmail.com.200106202429&email_token=j79QCzTbKQnuOk65WIs025wJMylfnWB3"}]}
)";
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

		struct curl_slist *header_chunk = NULL;

		std::string secret_line = "Authorization: Bearer " + secret;
    header_chunk = curl_slist_append(header_chunk, secret_line.c_str());
    header_chunk = curl_slist_append(header_chunk, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_chunk);


    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

  std::string buffer(chunk.memory);
  std::cout << "buffer: [" << buffer << "]\n";

  return 0;
}
