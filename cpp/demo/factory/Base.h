#ifndef __BASE_H
#define __BASE_H

#include <string>

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

#endif
