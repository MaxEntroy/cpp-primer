#include <initializer_list>
#include <iostream>
#include <vector>

typedef std::vector<int> VecInt;

VecInt* Factory(std::initializer_list<int> il = {});

void GetInput(VecInt* vi_ptr);

void Print(VecInt* vi_ptr);

int main(void) {
  VecInt* vi_ptr = Factory();
  if (!vi_ptr) {
    std::cerr << "Not enough room for VecInt." << std::endl;
    return 1;
  }

  GetInput(vi_ptr);

  Print(vi_ptr);

  delete vi_ptr;
  vi_ptr = nullptr;

  return 0;
}

VecInt* Factory(std::initializer_list<int> il) {
  VecInt* p = new (std::nothrow) VecInt(il);
  return p;
}

void GetInput(VecInt* vi_ptr) {
  int val = 0;
  while(std::cin >> val) {
    vi_ptr->push_back(val);
  }
}

void Print(VecInt* vi_ptr) {
  auto b = vi_ptr->begin();
  auto e = vi_ptr->end();

  auto i = b;
  while (i != e) {
    std::cout << *i << std::endl;
    ++i;
  }
}
