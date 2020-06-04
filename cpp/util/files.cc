#include <fstream>
#include <iostream>
#include <string>

#include "absl/strings/str_split.h"
#include "util/files.h"
#include "util/logging.h"
#include <sys/stat.h>

namespace vectorbook {

namespace {

bool FileExistsImpl(const char *file) {
  struct stat buf;
  return (stat(file, &buf) == 0);
}

} // namespace

bool FileExists(const std::string &file_name) {
  return FileExistsImpl(file_name.c_str());
}

namespace {

Void WriteStringToFile(const string &contents, const string &name) {
  FILE *file = fopen(name.c_str(), "wb");
  if (file == NULL) {
    RAISE_EXCEPTION("File null.");
  }
  if (fwrite(contents.data(), 1, contents.size(), file) != contents.size()) {
    fclose(file);
    RAISE_EXCEPTION("Failed to write file.");
  }
  if (fclose(file) != 0) {
    RAISE_EXCEPTION("Failed to close file.");
  }
  return Ok();
}

} // namespace

Void WriteFile(const std::string &file_name, const std::string &contents) {
  FILE *file = fopen(file_name.c_str(), "wb");
  if (file == NULL) {
    RAISE_EXCEPTION("File null.");
  }
  if (fwrite(contents.data(), 1, contents.size(), file) != contents.size()) {
    fclose(file);
    RAISE_EXCEPTION("Failed to write file.");
  }
  if (fclose(file) != 0) {
    RAISE_EXCEPTION("Failed to close file.");
  }
  return Ok();
}

Void WriteLines(const string &file_name, const std::vector<string> &contents) {
  ASSERT_SUCCEEDS(WriteFile(file_name, Join(contents, "")));
	return Ok();
}

Option<std::string> ReadFile(const std::string &file_name) {
  if (!FileExists(file_name)) {
		RAISE_EXCEPTION("File name does not exist: " + file_name);
	}
  string buffer;
  buffer.assign(
      std::istreambuf_iterator<char>(std::ifstream(file_name).rdbuf()),
      std::istreambuf_iterator<char>());
  return buffer;
}

Option<std::vector<std::string>> ReadLines(const std::string &file_name) {
  auto buffer = ReadFile(file_name);
  ASSERT_OK(buffer);
  return std::vector<std::string>(absl::StrSplit(*buffer, '\n'));
}

Option<std::string> ReadSecretFromFile(const std::string &file_name) {
  auto contents = ReadFile(file_name);
  ASSERT_OK(contents);
  return Trim(*contents);
}

} // namespace vectorbook
