#ifndef util_strings_h
#define util_strings_h
#include "absl/strings/str_cat.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
namespace vectorbook {
using absl::StrCat;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// Custom types.
using StringPairs = vector<pair<string, string>>;

using strings = vector<string>;

string Trim(const string &original);
template <typename COLN> string Join(const COLN &original, const string &s) {
  string buffer;
  // for (int i = 0; i < original.size(); ++i) {
	int i = 0;
	for (const auto& part : original) {
    if (i > 0) {
      buffer += s;
    }
    buffer += StrCat(part); // original[i];
		++i;
  };
  return buffer;
}

template <typename COLN> string Join(const COLN &original, char s) {
  return Join(original, string(s));
}

bool StartsWith(const string &haystack, const string &needle);
bool EndsWith(const string &haystack, const string &needle);

vector<string> Split(const string& input);
vector<string> StrSplit(const string& input);
vector<string> Split(const string &original, char c);
vector<string> Split(const string &original, const string &c);
bool Contains(const string &haystack, const string &needle);
string Replace(const string &original, const unordered_map<string, string> &mapping);
// string Replace(const string& original, const StringPairs& mapping);

std::string StrReplaceAll2(
    const string &s,
    std::initializer_list<std::pair<std::string, std::string>> replacements);

string SubString(const std::string &base, int start, int end);

bool Empty(const string &original);
bool NonEmpty(const string &original);

} // namespace vectorbook

#endif /* util_strings_h */
