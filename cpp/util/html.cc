#include "util/html.h"

namespace vectorbook {

const string kCss = R"(
<link rel="stylesheet" type="text/css" href="static/vectorbook.css">
)";

Void AddStandardHeaders(Html *html) {
  auto head = html->open_head(); ASSERT_OK(head);
  head->push_back(Html::JQueryLink());
  head->push_back(Html::JSLink("/static/label.js"));
  head->push_back(kCss);
  return Ok();
}

Unique<Html> StandardHtml() {
	auto html = Html::Root(); ASSERT_OK(html);
	auto add = AddStandardHeaders(html.get()); ASSERT_OK(add);
	return html;
}

Unique<Html> HtmlNode(const string& label) {
	return Html::Node(label);
}

} // namespace vectorbook
