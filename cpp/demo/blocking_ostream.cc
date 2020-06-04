#include <iostream>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "util/includes.h"

using namespace vectorbook;

// Empty class?
class MyStream {};

MyStream &InfoStream() {
  static MyStream *single = new MyStream();
  return *single;
}

template <typename T> MyStream &operator<<(MyStream &os, const T &t) {
  std::cout << t;
  return os;
}

MyStream &operator<<(MyStream &os, const string &t) {
  std::cout << t;
  return os;
}

class SuperStream {
public:
	string message_;
};

template <typename T> SuperStream &operator<<(SuperStream &os, const T &t) {
  os.message_ = StrCat(os.message_, t);
  return os;
}

SuperStream &operator<<(SuperStream &os, const string &t) {
  os.message_ = StrCat(os.message_, t);
  return os;
}

SuperStream &InfoSuperStream() {
  return SuperStream();
}

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  InfoSuperStream() << " hey there! " << 1 << '\n';

	Info() << C

  return 0;
}
