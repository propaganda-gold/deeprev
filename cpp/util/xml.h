#ifndef util_xml_h
#define util_xml_h
#include "util/includes.h"

namespace vectorbook {
namespace xml {

// Finds all the tags in XML document 'content' have tag 'tag'.
vector<string> FindAllSimpleTags(const string &tag, const string &content);

// Can parse a simple tag (e.g., meta, img).
optional<string> GetSimpleProperty(const string &simple_tag, const string &key);

// Finds the first bounded span with tag 'tag'. Eg: for tag="title":
//    "<title>hello there</title>"
//
optional<string> FindBounded(const string &tag, const string &content);

// Gets the part inside of simple tags, e.g., "hello there" in:
//  <title>hello there</title>
optional<string> ValueInsideTags(const string &content);

Unique<Seq<string>> AllLinks(const string& content);

} // namespace xml
} // namespace vectorbook
#endif /* util_xml_h */
