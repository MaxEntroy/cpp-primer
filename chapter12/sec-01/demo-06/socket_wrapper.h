#ifndef SOCKET_WRAPPER_H_
#define SOCKET_WRAPPER_H_

#include <iostream>

#include <unistd.h>

namespace cp {

class SocketWrapper {
 public:
   SocketWrapper() : sfd_(0) {}
   explicit SocketWrapper(int sfd) : sfd_(sfd) {}
   ~SocketWrapper() {
     close(sfd_);
     std::cout << "~SocketWrapper() called." << std::endl;
   }

 public:
   int get() const;

 private:
   int sfd_;
};


} // namespace cp

#endif // SOCKET_WRAPPER_H_
