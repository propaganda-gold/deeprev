#include "util/xml.h"

#include "coln/util.h"
#include "util/includes.h"
#include "util/rapidxml.hpp"

namespace vectorbook {
namespace xml {
using namespace rapidxml;

vector<string> FindAllSimpleTags(const string &tag, const string &content) {
  string effective_tag = "<" + tag + " ";
  string buffer(content);
	vector<string> result;
  int i = 0;
  std::size_t pos = 0;
  while (true) {
    std::size_t l_pos = buffer.find(effective_tag, pos + 1);
    if (l_pos == std::string::npos) {
      break;
    }
    std::size_t r_pos = buffer.find(">", l_pos + 1);
    if (r_pos == std::string::npos) {
      break;
    }

    const string sub = buffer.substr(l_pos, r_pos - l_pos + 1);
		result.push_back(sub);

    pos = r_pos;
    ++i;
    if (i > 125)
      break;
  }

  return result;
}

optional<string> GetSimpleProperty(const string &simple_tag, const string& key) {
	string partition = StrCat(key, "=\"");
	vector<string> parts = Split(simple_tag, partition);
	if (parts.size() < 2) {
		return nullopt;
	}
	vector<string> parts2 = Split(parts[1], "\"");
	if (parts.size() < 2) {
		return nullopt;
	}
  return parts2[0];
}

optional<string> FindBounded(const string &tag, const string &content) {
  std::size_t l_pos = content.find("<" + tag);
  if (l_pos == std::string::npos) {
    return nullopt;
  }
  std::size_t r_pos = content.find("</" + tag, l_pos + 1);
  if (r_pos == std::string::npos) {
    return nullopt;
  }
  std::size_t right_index = r_pos + tag.size() + 3;
  return content.substr(l_pos, right_index - l_pos + 1);
}

optional<string> ValueInsideTags(const string &content) {
  vector<string> parts1 = Split(content, ">");
  if (parts1.size() < 2)
    return nullopt;
  string &tail = parts1[1];
  vector<string> parts2 = Split(tail, "<");
  return parts2[0];
}

Unique<Seq<string>> AllLinks(const string& content) {
	vector<string> ref_tags = {"href", "HREF"};
	auto buffer = new StdSeq<string>();
	StdSeq<string> tags;
	AppendAll(&tags, xml::FindAllSimpleTags("A", content));
	AppendAll(&tags, xml::FindAllSimpleTags("a", content));
	for(auto tag : tags) {
		for (string& ref_tag : ref_tags) {
			auto href = GetSimpleProperty(*tag, ref_tag);
			if (href.has_value()) {
				buffer->append(*href);
				break;
			}
		}
	}

	return buffer;
}

} // namespace xml
} // namespace vectorbook
