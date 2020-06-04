#ifndef UTIL_HTML
#define UTIL_HTML
#include "util/includes.h"
#include "util/strings.h"
#include <memory>
#include <string>
namespace vectorbook {
using std::string;
using std::unique_ptr;
using std::vector;
class Html {
public:
  static Unique<Html> Leaf(const string &tag) {
    return Unique<Html>(new Html(tag, ""));
  }
  static Unique<Html> Node(const string &tag) {
    return Unique<Html>(new Html("", tag));
  }
  static Unique<Html> Root() { return Node("<html>"); }
  Void push_back(const string &tag) { parts_.push_back(Html::Leaf(tag)); return Ok();}
  Void push_back(
      const string &tag,
      std::initializer_list<std::pair<std::string, std::string>> replacements) {
    string replaced = StrReplaceAll2(tag, replacements);
    push_back(replaced);
		return Ok();
  }
  Void push_back(unique_ptr<Html> &&other) {
    parts_.push_back(std::move(other));
		return Ok();
  }
  Void push_back(Unique<Html> &&other) {
		std::unique_ptr<Html> temp (other.release());
    parts_.push_back(std::move(temp));
		return Ok();
  }
  Mutable<Html> open(const string &open_tag) {
    auto part = Html::Node(open_tag);
    parts_.push_back(std::move(part));
    return parts_.back().get();
  }
  Mutable<Html> open_head() { return open("<head>"); }
  Mutable<Html> open_body() { return open("<body>"); }
  Unique<string> Build() const {
    auto buffer = new string();
    BuildInternal(buffer);
    return buffer;
  }

  Void add_title(const string &title) {
    push_back(Replace("<title>$title</title>", {{"$title", title}}));
		return Ok();
  }

  static Unique<Html> JSLink(const string &url) {
    string script = StrReplaceAll2(R"(
<script type="text/javascript" src="$url"></script>
		)",
                                   {{"$url", url}});
    return Leaf(script);
  }
  static Unique<Html> JQueryLink() {
    string script = StrReplaceAll2(R"(
	<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
		)",
                                   {});
    return Leaf(script);
  }

private:
  Html(const string &tag, const string &open_tag)
      : tag_(tag), open_tag_(open_tag) {}
  Void BuildInternal(string *buffer) const {
    if (!tag_.empty()) {
      *buffer += tag_;
    } else {
      *buffer += open_tag_;
      for (const auto &part : parts_) {
				auto build_part = part->Build(); ASSERT_OK(build_part);
        *buffer += *build_part;
      }
      *buffer += GetCloseTag(open_tag_);
    }
		return Ok();
  }

  string tag_;
  string open_tag_;
  vector<Unique<Html>> parts_;
  static string GetCloseTag(const string &open_tag) {
    string buffer;
    buffer += "</";
    for (int i = 1; i < open_tag.size(); ++i) {
      const char c = open_tag[i];
      const bool lower_char = c >= 'a' && c <= 'z';
      const bool higher_char = c >= 'A' && c <= 'Z';
      if (lower_char || higher_char) {
        buffer += c;
      } else {
        break;
      }
    }
    buffer += ">";
    return buffer;
  }
};

Void AddStandardHeaders(Html* html);

Unique<Html> StandardHtml();

Unique<Html> HtmlNode(const string& label);

} // namespace vectorbook
#endif /* UTIL_HTML */
