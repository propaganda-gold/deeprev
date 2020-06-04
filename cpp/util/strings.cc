#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "absl/strings/ascii.h"
#include "absl/strings/match.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/substitute.h"
#include "util/logging.h"
#include "util/strings.h"

namespace vectorbook {

string Trim(const string &original) {
  return string(absl::StripAsciiWhitespace(original));
}

vector<string> Split(const string &original, char c) {
  return absl::StrSplit(original, c);
  // vector<string> buffer; return buffer;
}

vector<string> Split(const string &original, const string &c) {
  return absl::StrSplit(original, c);
  // vector<string> buffer; return buffer;
}

bool StartsWith(const string &haystack, const string &needle) {
  return absl::StartsWith(haystack, needle);
}
bool EndsWith(const string &haystack, const string &needle) {
  return absl::EndsWith(haystack, needle);
}

bool Contains(const string &haystack, const string &needle) {
  return haystack.find(needle) != std::string::npos;
}

string Replace(const string &original, const unordered_map<string, string> &mapping) {
  return absl::StrReplaceAll(original, mapping);
}

string Replace(const string &original, const StringPairs &mapping) {
  return absl::StrReplaceAll(original, mapping);
}

std::string SubString(const std::string &base, int start, int end) {
  std::string buffer;
  for (unsigned int i = start; i < end; ++i) {
    if (i >= 0 && i < base.size()) {
      buffer += base[i];
    }
  }
  return buffer;
}

bool Empty(const string &original) { return original.empty(); }
bool NonEmpty(const string &original) { return !original.empty(); }

std::string StrReplaceAll2(
    const string &s,
    std::initializer_list<std::pair<std::string, std::string>> replacements) {
  return absl::StrReplaceAll(s, replacements);
}

vector<string> Split(const string &input) {
  std::istringstream buffer(input);
  std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)),
                               std::istream_iterator<std::string>());
  return ret;
}

vector<string> StrSplit(const string &input) { return Split(input); }

} // namespace vectorbook
