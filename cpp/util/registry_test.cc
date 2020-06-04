#include "util/registry.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(Registry, Basic) {
  Base *l = BaseFactory::Create("B");
  if (l != NULL) {
    B *b = dynamic_cast<B *>(l);
    printf("%s %s registered.\n", b->GetName().c_str(),
           b == nullptr ? "null" : "exists");
    delete l;
  } else {
    printf("B doesn't exist.\n");
  }
  l = BaseFactory::Create("D");
  if (l != NULL) {
    printf("%s %s registered.\n", l->GetName().c_str(),
           l->IsRegistered() ? "is" : "is not");
    delete l;
  } else {
    printf("D doesn't exist.\n");
  }
}

} // namespace vectorbook
