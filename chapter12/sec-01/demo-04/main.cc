#include <initializer_list>
#include <iostream>
#include <memory>
#include <vector>

typedef std::vector<int> VecInt;

std::shared_ptr<VecInt> Factory(std::initializer_list<int> il = {});

void GetInput(std::shared_ptr<VecInt> vi_ptr);

void Print(std::shared_ptr<VecInt> vi_ptr);

int main(void) {
  std::shared_ptr<VecInt> vi_ptr = Factory();

  GetInput(vi_ptr);

  Print(vi_ptr);

  return 0;
}

std::shared_ptr<VecInt> Factory(std::initializer_list<int> il) {
  return std::make_shared<VecInt>(il);
}

void GetInput(std::shared_ptr<VecInt> vi_ptr) {
  int val = 0;
  while(std::cin >> val) {
    vi_ptr->push_back(val);
  }
}

void Print(std::shared_ptr<VecInt> vi_ptr) {
  for(const auto& i : *vi_ptr) {
    std::cout << i << std::endl;
  }
}
