#include "util/xml.h"

#include "util/download.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {
namespace xml {

TEST(AllLinks, Basic) {
	auto content = R"(
		<a href="link1">
		<A href="link1">
		<A HREF="link1">
		<a HREF="link1">
	)";

	auto links = AllLinks(content);
	ASSERT_TRUE(links.ok());
	for (auto it : *links) {
		Output_value(*it);
	}
}

//TEST(FindAllSimpleTags, Example) {
//  string url = "https://www.dcclothesline.com/2019/09/21/"
//               "twitter-suspends-entire-leadership-team-of-latinos-for-trump/";
//  string html_content;
//  ASSERT_TRUE(Assign(DownloadHttpPage(url), &html_content).ok());
//  // Output_() << html_content << endl;
//  {
//    vector<string> all = FindAllSimpleTags("img", html_content);
//  }
//  {
//    vector<string> all = FindAllSimpleTags("meta", html_content);
//  }
//}
//
//TEST(GetSimpleProperty, Example) {
//	optional<string> prop = GetSimpleProperty(R"(<img content="hi-there.jpg" />)", "content");
//	if (prop.has_value()) {
//	}
//}
//
//TEST(FindBounded, Example) {
//  string url = "https://www.dcclothesline.com/2019/09/21/"
//               "twitter-suspends-entire-leadership-team-of-latinos-for-trump/";
//  string html_content;
//  ASSERT_TRUE(Assign(DownloadHttpPage(url), &html_content).ok());
//	optional<string> title = FindBounded("title", html_content);
//	if (title.has_value()) {
//	}
//}
//
//TEST(ValueInsideTags, Example) {
//	string content = "<title>hello world</title>";
//	optional<string> inside = ValueInsideTags(content);
//	if (inside.has_value()) {
//	}
//}

} // namespace xml
} // namespace vectorbook
