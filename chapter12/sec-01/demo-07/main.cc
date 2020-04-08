#include <iostream>
#include <memory>

#include <sys/socket.h>
#include <unistd.h>

void close_sfd(int*);
void test_socket();

int main(void) {
  while(true) {
    test_socket();
    sleep(1);
  }
  return 0;
}

void close_sfd(int* p_sfd) {
  close(*p_sfd);
  std::cout << "close_sfd() called." << std::endl;
}

void test_socket() {
  int sfd = socket(PF_INET, SOCK_STREAM, 0);
  std::unique_ptr<int, decltype(close_sfd)*> sfd_ptr(&sfd, close_sfd);
}
