#include <memory>
#include "derived.h"
using namespace test;

// print with ordinary pointer
void print_op(Base* ptr) {
  ptr->print();
}

// print with smart pointer
void print_sp(std::shared_ptr<Base> ptr) {
  ptr->print();
}

// stack object
void test1() {
  Base base;
  Derived derived;

  print_op(&base);
  print_op(&derived);

  // compiler will not complain, but is is undefined behavior.
  // print_sp(std::shared_ptr<Base>(&base));
  // print_sp(std::shared_ptr<Base>(&derived));
}

// dynamic object managed with original pointer
void test21() {
  Base* p_base = new Base();
  Derived* p_derived = new Derived();

  print_op(p_base);
  print_op(p_derived);

  delete p_base; p_base = nullptr;
  delete p_derived; p_derived = nullptr;
}

// dynamic object managed with original pointer
// Not recommended! Doing so is likely to be an error.
void test22() {
  Base* p_base = new Base();
  Derived* p_derived = new Derived();

  print_sp(std::shared_ptr<Base>(p_base)); // legal, but memory will be deleted.
  print_sp(std::shared_ptr<Derived>(p_derived));

  // Now p_base is a dangling pointer, and you may not realize it
  // delelte p_base; // it is undefined.
}

// dynamic object managed with smart pointer
void test31() {
  std::shared_ptr<Base> base_ptr = std::make_shared<Base>();
  std::shared_ptr<Derived> derived_ptr = std::make_shared<Derived>();

  print_op(base_ptr.get());
  print_op(derived_ptr.get());
}

void test32() {
  std::shared_ptr<Base> base_ptr = std::make_shared<Base>();
  std::shared_ptr<Derived> derived_ptr = std::make_shared<Derived>();

  print_sp(base_ptr);
  print_sp(derived_ptr);
}

int main(void) {
  test1();
  test21();
  test22();
  test31();
  test32();
  return 0;
}
