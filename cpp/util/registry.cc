#include "util/registry.h"

#include "util/includes.h"
#include <cstdio>
#include <utility>

namespace vectorbook {

Base::Base() : IsRegistered_(false) {}

Base::Base(bool isRegistered) : IsRegistered_(isRegistered) {}

bool Base::IsRegistered() const { return this->IsRegistered_; }

bool BaseFactory::Register(std::string name, FactoryFn createMethod) {
  printf("Registering %s\n", name.c_str()); // debugging purposes
  // add the pair to the map
  std::pair<std::map<std::string, FactoryFn>::iterator, bool> registeredPair =
      BaseFactory::RegisteredNames.insert(
          std::make_pair(name.c_str(), createMethod));
  // return whether it was added or updated
  return registeredPair.second;
}

Base *BaseFactory::Create(std::string name) {
  // attempt to get the pair from the map
  std::map<std::string, FactoryFn>::iterator registeredPair =
      BaseFactory::RegisteredNames.find(name);
  // did we find one?
  if (registeredPair == BaseFactory::RegisteredNames.end())
    return NULL; // return NULL
  // return a new instance of derived class
  return registeredPair->second();
}

// initialise the registered names map
std::map<std::string, FactoryFn> BaseFactory::RegisteredNames = {};

} // namespace vectorbook
