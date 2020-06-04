#ifndef __BASE_IMPL_H
#define __BASE_IMPL_H

#include <string>

typedef Base* (*factoryMethod)();

template <typename T>
class BaseFactory {
public:
  static bool Register(std::string name, factoryMethod createMethod);
  static T *Create(std::string name);

protected:
  static std::map<std::string, factoryMethod> RegisteredNames;
};

template<typename T>
class BaseImpl : public Base {
public:
  // give derived classes the ability to create themselves
  static Base *Create() { return new T(); }
  // get the identifier of the derived class
  std::string GetName() {
    return T::Name;
  }

protected:
  // to determine if the class definition is registered
  static const bool IsRegistered_;

  BaseImpl() : 
    Base(IsRegistered_) { }
};

template<typename T>
// attempt to initialise the IsRegistered variable of derived classes
// whilst registering them to the factory
const bool BaseImpl<T>::IsRegistered_ =
    BaseFactory::Register(T::Name, &BaseImpl<T>::Create);

#endif
