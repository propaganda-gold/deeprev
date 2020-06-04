#include "util/check_recaptcha.h"

#include "curl/curl.h"
#include "server/options.h"
#include "util/includes.h"
#include "util/smtp.pb.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

namespace vectorbook {

namespace {

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

} // namespace

Option<vectorbook::RecaptchaResponse>
CheckRecaptcha(const RecaptchaRequest &request) {
  const string &secret = request.secret();
  const string &token = request.token();
  if (token.empty() || secret.empty()) {
    RAISE_EXCEPTION("some input is empty: token [" + token + "], secret [" +
                    secret + "]\n");
  }
  CURL *curl;
  CURLcode res;

  struct MemoryStruct chunk;
  chunk.memory = (char *)malloc(1);
  chunk.size = 0;

  /* get a curl handle */
  curl = curl_easy_init();
  if (curl) {
    /* First set the URL that is about to receive our POST. */
    curl_easy_setopt(curl, CURLOPT_URL,
                     "https://www.google.com/recaptcha/api/siteverify");

    /* Now specify we want to POST data */
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    /* get verbose debug output please */
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    std::string post_data;
    post_data += "secret=" + secret;
    post_data += "&";
    post_data += "response=" + token;
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

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

  std::string buffer(chunk.memory);
  std::cout << buffer << "\n";

  vectorbook::RecaptchaResponse captcha_result;
  auto status =
      google::protobuf::util::JsonStringToMessage(buffer, &captcha_result);
  std::cout << "status: " << status << "\n";
  return captcha_result;
}

Boolean GetRecaptchaSuccess(const string &token) {
  RecaptchaRequest request;
  request.set_secret(statics::secrets().recaptcha_api());
  request.set_token(token);
  auto result = CheckRecaptcha(request);
  ASSERT_EXISTS(result);

  if (!result->success()) {
    return false;
  }

  if (result->score() < 0.7) {
    return false;
  }

  return true;
}

} // namespace vectorbook
