#include "demo/factory/Base.h"

Base::Base() :
  IsRegistered_(false)
{ }

Base::Base(bool isRegistered) :
  IsRegistered_(isRegistered)
{ }

bool
Base::IsRegistered() const {
  return this->IsRegistered_;
}
