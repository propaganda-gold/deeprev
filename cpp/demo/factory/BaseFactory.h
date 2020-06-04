#ifndef __BASE_FACTORY_H
#define __BASE_FACTORY_H

#include "Base.h"
#include <string>
#include <map>

typedef Base* (*factoryMethod)();

class BaseFactory {
public:
  // register a class name with a particular create method
  static bool Register(std::string name, factoryMethod createMethod);
  // create a derived class, given a name
  static Base *Create(std::string name);

protected:
  // a map to hold a ... mapping between strings and create functions

	// Note!.. static map!
  static std::map<std::string, factoryMethod> RegisteredNames;
};

#endif
