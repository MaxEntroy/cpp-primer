#include "str_blob.h"
#include "weak_str_blob.h"

using namespace cp;

int main(void) {
  StrBlob strblob;
  strblob.push_back("hello");
  strblob.push_back("world");

  int sz = strblob.size();

  WeakStrBlob wstrblob(strblob);

  for(int i = 0; i < sz; ++i) {
    std::cout << wstrblob.deref() << std::endl;
    if(i == sz - 1) break;
    wstrblob.incr();
  }

  return 0;
}
