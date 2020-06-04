#include "neural/action_converter.h"

#include "util/includes.h"

namespace vectorbook {

Feature CreateFeature(const string &name, const string &value,
                      const string &embedding_group) {
  Feature result;
  result.set_name(name);
  result.set_value(value);
  result.set_embedding_group(embedding_group);
  return result;
}

} // namespace vectorbook
