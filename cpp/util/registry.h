#ifndef util_registry_h
#define util_registry_h
#include "util/includes.h"

namespace vectorbook {

class Base {
public:
  Base();
  Base(bool isRegistered);
  virtual ~Base() {}
  // to determine if this instance class is an instance
  // of a derived class, registered to the factory.
  bool IsRegistered() const;
  // a way for derived classes to identify themselves
  virtual std::string GetName() = 0;
    
private:
  const bool IsRegistered_;
};

typedef Base* (*FactoryFn)();

template <typename T>
class BaseFactory {
public:
	static bool Register(std::string name, FactoryFn createMethod) {
		printf("Registering %s\n", name.c_str()); // debugging purposes
		// add the pair to the map
		std::pair<std::map<std::string, FactoryFn>::iterator, bool> registeredPair =
				BaseFactory::RegisteredNames.insert(
						std::make_pair(name.c_str(), createMethod));
		// return whether it was added or updated
		return registeredPair.second;
	}

	static T *Create(std::string name) {
		// attempt to get the pair from the map
		std::map<std::string, FactoryFn>::iterator registeredPair =
				BaseFactory::RegisteredNames.find(name);
		// did we find one?
		if (registeredPair == BaseFactory::RegisteredNames.end())
			return NULL; // return NULL
		// return a new instance of derived class
		return registeredPair->second();
	}

protected:
  // a map to hold a ... mapping between strings and create functions

	// Note!.. static map!
  static std::map<std::string, FactoryFn> RegisteredNames;
};

template <typename T> class BaseImpl : public Base {
public:
  static Base *Create() { return new T(); }
  std::string GetName() { return T::Name; }

protected:
  static const bool IsRegistered_;
	bool registered_;

  BaseImpl() : Base(IsRegistered_) {}
};

template <typename T>
const bool BaseImpl<T>::IsRegistered_ =
    BaseFactory<T>::Register(T::Name, &BaseImpl<T>::Create);

#define X(x)                                                                   \
  class x : public BaseImpl<x> {                                               \
  public:                                                                      \
    x() : BaseImpl<x>() {}                                                     \
    static const std::string Name;                                             \
  };                                                                           \
  const std::string x::Name = #x;

X(A)
X(B)
X(C)

#undef X


} // namespace vectorbook
#endif /* util_registry_h */
