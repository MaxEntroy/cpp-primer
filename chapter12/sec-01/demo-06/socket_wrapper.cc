#include "socket_wrapper.h"

namespace cp {

int SocketWrapper::get() const {
  return sfd_;
}

} // namespace cp
