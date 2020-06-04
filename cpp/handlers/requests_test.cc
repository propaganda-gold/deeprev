#include "handlers/requests.h"

#include "absl/strings/str_join.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
namespace vectorbook {

//TEST(ConvertHttpRequest, Basic) {
//	const vector<string> parts = {
//	"GET http://www.w3.org/pub/WWW/TheProject.html?hello=there&now=then HTTP/1.1",
//	"Cookie: hello=there;",
//	"",
//	"This is the body"
//	};
//	const string original = StrJoin(parts, "\n");
//	HttpRequest request = ConvertHttpRequest(original);
//}

TEST(ConvertHttpResponse, Json) {
	auto response = ParseProto<HttpResponse>(R"(
		code: 200
		json {
			status: "ASSERT_OK"
		}
	)");

	Unique<string> s = ConvertHttpResponse(response);
	ASSERT_TRUE(s.ok());
}

} // namespace redis
