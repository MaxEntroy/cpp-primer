#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include <mutex>
#include <string>

namespace cp {

class Employee {
 public:
  Employee() : uid_(incr()) {}
  explicit Employee(const std::string& s) : name_(s), uid_(incr()) {}

  Employee(const Employee&);
  Employee& operator=(const Employee&);

 public:
  std::string name() const {return name_;}
  int uid() const {return uid_;}

 private:
  static int incr();

 private:
  std::string name_;
  int uid_;

  static int counter_;
  static std::mutex mtx;
};

} // namesapce cp

#endif // EMPLOYEE_H_
