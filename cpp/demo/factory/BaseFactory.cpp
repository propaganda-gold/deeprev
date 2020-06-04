#include "demo/factory/BaseFactory.h"

#include <utility>
#include <cstdio>

bool
BaseFactory::Register(std::string name, factoryMethod createMethod) {
  printf("Registering %s\n", name.c_str()); // debugging purposes
  // add the pair to the map
  std::pair<std::map<std::string, factoryMethod>::iterator, bool> registeredPair =
      BaseFactory::RegisteredNames.insert(std::make_pair(name.c_str(), createMethod));
  // return whether it was added or updated
  return registeredPair.second;
}

Base *
BaseFactory::Create(std::string name) {
  // attempt to get the pair from the map
  std::map<std::string, factoryMethod>::iterator registeredPair =
  BaseFactory::RegisteredNames.find(name);
  // did we find one?
  if(registeredPair == BaseFactory::RegisteredNames.end())
    return NULL; // return NULL
  // return a new instance of derived class
  return registeredPair->second();
}

// initialise the registered names map
std::map<std::string, factoryMethod> BaseFactory::RegisteredNames = { };
