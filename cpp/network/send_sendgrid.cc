#include "network/send_sendgrid.h"

#include "curl/curl.h"
#include "util/includes.h"

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

Option<string> CreateMessage(const SendgridRequest &request) {
  const string template_ =
      R"({"personalizations": [{"to": [{"email": "$to_address"}]}],"from": {"email": "$from_address", "name":"$from_name"},"subject": "$subject","content": [{"type": "text/html", "value": "$body"}]}
)";

        return Replace(template_, {
		{"$to_address", request.to_address()},
		{"$from_address", request.from_address()},
		{"$from_name", request.from_name()},
		{"$subject", request.subject()},
		{"$body", request.body()},
	});
}

} // namespace

Void SendSendgrid(const SendgridRequest &request) {
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
		//    fprintf(stderr, "curl_global_init() failed: %s\n", curl_easy_strerror(res));
		RAISE_EXCEPTION(StrCat("cur_init returned an error: ", curl_easy_strerror(res)));
  }

  /* get a curl handle */
  curl = curl_easy_init();
  if (curl) {
		// IMPORTANT: Use "https" and not "http".
    curl_easy_setopt(curl, CURLOPT_URL,
                     "https://api.sendgrid.com/v3/mail/send");

    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		auto post_data = CreateMessage(request); ASSERT_EXISTS(post_data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data->c_str());

    struct curl_slist *header_chunk = NULL;

    std::string secret_line = "Authorization: Bearer " + request.secret();
    header_chunk = curl_slist_append(header_chunk, secret_line.c_str());
    header_chunk =
        curl_slist_append(header_chunk, "Content-Type: application/json");
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
  return Ok();
}

} // namespace vectorbook
