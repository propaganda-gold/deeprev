#include "BaseImpl.h"
#include "BaseFactory.h"
#include <string>

#define X(x) \
class x : public BaseImpl<x> {\
public:\
  x() : BaseImpl<x>() {}\
  static const std::string Name;\
};\
const std::string x::Name = #x;

X(A)
X(B)
X(C)

#undef X

int
main(void)
{
  Base *l = BaseFactory::Create("B");
  if(l != NULL) {
    B *b = dynamic_cast<B*>(l);
    printf("%s %s registered.\n", b->GetName().c_str(), b->IsRegistered() ? "is" : "is not");
    delete l;
  } else {
    printf("B doesn't exist.\n");
  }
  l = BaseFactory::Create("D");
  if(l != NULL) {
    printf("%s %s registered.\n", l->GetName().c_str(), l->IsRegistered() ? "is" : "is not");
    delete l;
  } else {
    printf("D doesn't exist.\n");
  }
  return 0;
}
