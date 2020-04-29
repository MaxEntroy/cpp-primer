#include "employee.h"

namespace cp {

int Employee::counter_ = 0;
std::mutex Employee::mtx;

Employee::Employee(const Employee& rhs) {
  name_ = rhs.name_;
  uid_ = incr();
}

Employee& Employee::operator=(const Employee& rhs) {
  name_ = rhs.name_;
  uid_ = incr();

  return *this;
}

int Employee::incr() {
  std::lock_guard<std::mutex> guard(mtx);
  return ++counter_;
}

}
