#include<iostream>
#include<regex>
#include<string>
#include "util/re.h"
#include "util/includes.h"

namespace vectorbook {
namespace re {
using namespace std;

vector<string> FindAllMatches(const string &haystack, const string &re) {
  smatch s_match;
  regex e("(" + re + ")");
	vector<string> result;
  if (regex_search(haystack, s_match, e)) {
    for (int i = 0; i < s_match.size(); ++i) {
			result.push_back(s_match[i]);
    }
  }
	return result;
}

}  // namespace re
}  // namespace vectorbook
