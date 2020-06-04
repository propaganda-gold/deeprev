#include "neural/api.h"

#include "util/includes.h"

namespace vectorbook {

DoubleVector RandomVector(size_t dimension) {
  DoubleVector result;
  for (size_t i = 0; i < dimension; ++i) {
    int v = rand() % 200 - 100;
    result.add_weight(0.01 * v);
  }
  return result;
}

} // namespace vectorbook
