#include <stdio.h>
#include <string.h>

#define BUF_SZ 128

void test();

int main() {

  test();

  return 0;
}

void test() {
  static char buf[BUF_SZ];

  while(fgets(buf, BUF_SZ - 1, stdin)) {
    size_t sz = strlen(buf);
    char* s = new char[sz];

    strncpy(s, buf, sz-1);
    s[sz-1] = '\0';

    printf("%s\n", s);

    delete [] s;
  }
}
