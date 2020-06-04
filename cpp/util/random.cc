#include <random>

#include "util/random.h"

namespace vectorbook {

std::string RandomString(const std::string &characters, int length) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(
      0, characters.size() - 1); // distribution range
  std::string buffer;
  for (int i = 0; i < length; ++i) {
    int ix = dist6(rng);
    if (ix < 0 || ix >= characters.size()) {
      continue;
    }
    char c = characters[ix];
    buffer += c;
  }
  return buffer;
}

std::string Random62String(size_t length) {
  return RandomString(
      "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", length);
}

std::string Random62String() { return Random62String(32); }

size_t RandomInt(size_t N) { return rand() % N; }

Unique<bool> RandomTestFloat(float point) {
	// HACK(greg)
  const float OKpoint =
      static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return new bool(OKpoint <= point);
}

} // namespace vectorbook
