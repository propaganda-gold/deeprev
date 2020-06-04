#include "handlers/requests.h"

#include "handlers/api.h"
#include "util/includes.h"

namespace vectorbook {
namespace {

void ParseGetFromUri(const string &uri, HttpRequest *request) {
  const vector<string> parts = Split(uri, "?");
  if (parts.size() < 2) {
    return;
  }
  const auto &back = parts.back();
  const vector<string> clauses = Split(back, "&");
  for (const auto &clause : clauses) {
    const vector<string> eq_parts = Split(clause, "=");
    if (eq_parts.size() == 2) {
      (*request->mutable_get())[eq_parts[0]] = UnescapeHttpString(eq_parts[1]);
    }
  }
}

void ParseFirstRequestLine(const string &line, HttpRequest *request) {
  const vector<string> parts = Split(line, " ");
  if (parts.size() > 0) {
    request->set_method(parts[0]);
  }
  if (parts.size() > 1) {
    const string &uri = parts[1];
    request->set_uri(uri);
    vector<string> rough_parts = Split(uri, "/");
    if (!rough_parts[0].empty()) {
      throw StrCat("first part not empty: ", uri);
    }
    for (int i = 1; i < rough_parts.size(); ++i) {
      auto left_part = Split(rough_parts[i], "?")[0];
      request->add_uri_part(left_part);
    }
    ParseGetFromUri(uri, request);
  }
  if (parts.size() > 2) {
    request->set_version(parts[2]);
  }
}

void ParseAllCookies(const vector<string> &lines, HttpRequest *request) {
  for (const string &line : lines) {
    const vector<string> parts = Split(line, "Cookie: ");
    if (parts.size() > 1) {
      const string &kv = parts[1];
      const vector<string> kvp = Split(kv, "=");
      if (kvp.size() > 1) {
        (*request->mutable_cookies())[kvp[0]] = Trim(kvp[1]);
      }
    }
  }
}

void ParseHeaders(const vector<string> &lines, HttpRequest *request) {
  for (int i = 1; i < lines.size(); ++i) {
    const auto &line = lines[i];
    if (Trim(line).empty()) {
      break;
    }
    const vector<string> parts = Split(line, ": ");
    if (parts.size() == 2) {
      (*request->mutable_headers())[parts[0]] = Trim(parts[1]);
    }
  }
}

void ParsePost(const vector<string> &lines, HttpRequest *request) {
  int line_idx = 0;
  for (; line_idx < lines.size(); ++line_idx) {
    if (Trim(lines[line_idx]).empty()) {
      break;
    }
  }
  for (; line_idx < lines.size(); ++line_idx) {
    string line = Trim(lines[line_idx]);
    vector<string> outer_parts = Split(line, "&");
    for (auto &outer_part : outer_parts) {
      vector<string> parts = Split(outer_part, '=');
      if (parts.size() == 2) {
        (*request->mutable_post())[parts[0]] =
            UnescapeHttpString(parts[1]);
      }
    }
  }
}

} // namespace

HttpRequest ConvertHttpRequest(const string &original) {
  const vector<string> lines = Split(original, "\n");
  HttpRequest result;
  result.set_original(original);
  if (lines.size() > 0) {
    ParseFirstRequestLine(lines[0], &result);
  }
  ParseAllCookies(lines, &result);
  ParseHeaders(lines, &result);
  ParsePost(lines, &result);
  return result;
}

namespace {

Option<string> ConvertJsonResponse(const HttpResponse &response) {
  string buffer;
	// TODO(greg) fix this.
	const string code_string = response.code() == 200 ? "OK" : "ERROR";
  buffer += StrCat("HTTP/1.1 ", response.code(), " ", code_string, "\r\n");
  buffer += "Content-Type: application/json; charset=utf-8\r\n";
	buffer += "Access-Control-Allow-Origin: *\r\n";
  buffer += StrCat("Content-Length: ", response.json().size(), "\r\n");
  for (const string &cookie : response.cookie()) {
    buffer += StrCat("Set-Cookie: ", cookie, "\r\n");
  }
  buffer += "\r\n";
  buffer += response.json();
  buffer += "\r\n";
  return buffer;
}

} // namespace

Option<string> ConvertHttpResponse(const HttpResponse &response) {
  if (response.has_json()) {
    return ConvertJsonResponse(response);
  }
  string buffer;
  buffer += StrCat("HTTP/1.1 ", response.code(), " ASSERT_OK\r\n");
  buffer += "Content-Type: text/html; charset=utf-8\r\n";
  buffer += StrCat("Content-Length: ", response.content().size(), "\r\n");
  for (const string &cookie : response.cookie()) {
    buffer += StrCat("Set-Cookie: ", cookie, "\r\n");
  }
  buffer += "\r\n";
  buffer += response.content() + "\r\n";
  return buffer;
}

Option<HttpResponse> MakeResponse(int code, const string &content) {
  HttpResponse r;
  r.set_code(code);
  r.set_content(content);
  return r;
}

} // namespace vectorbook
