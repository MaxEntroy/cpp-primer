#include <iostream>
#include <memory>
#include <string>

void test();
void test1();

int main(void) {
  // test();
  test1();

  return 0;
}

void test() {
  std::string s;
  std::allocator<std::string> alloc;
  while(std::cin >> s) {
    auto s_ptr = alloc.allocate(1);
    alloc.construct(s_ptr, std::string(s.rbegin(), s.rend()));

    std::cout << *s_ptr << std::endl;

    alloc.destroy(s_ptr);
    alloc.deallocate(s_ptr, 1);
  }
}

void test1() {
  std::string s;
  std::allocator<char> alloc;
  while(std::cin >> s) {
    int sz = s.size();
    auto s_ptr = alloc.allocate(sz + 1);

    std::uninitialized_copy(s.rbegin(), s.rend(), s_ptr);
    alloc.construct(s_ptr + sz, '\0');

    std::cout << s_ptr << std::endl; // 打印c-style string, 打印首地址才行，*s_ptr是首个字符元素。因为c-style string本质是char [n];

    for(int i = 0; i < sz + 1; ++i) {
      alloc.destroy(s_ptr + i);
    }

    alloc.deallocate(s_ptr, sz + 1);
  }
}
