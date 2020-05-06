#include "message.h"

namespace cp {

Message::~Message() {
  BatchRemove();
}

Message::Message(const Message& rhs) {
  msg_ = rhs.msg_;
  BatchAdd(rhs.pfolder_list_);
}

Message& Message::operator=(const Message& rhs) {
  if (this == &rhs) {
    return *this;
  }

  BatchRemove();

  msg_ = rhs.msg_;
  BatchAdd(rhs.pfolder_list_);

  return *this;
}

void Message::AddFolder(Folder* pfolder) {
  pfolder_list_.push_back(pfolder);
  pfolder->AddMsg(this);
}

void Message::RemoveFolder(Folder* pfolder) {
  pfolder_list_.remove(pfolder);
  pfolder->RemoveMsg(this);
}

void Message::BatchAdd(const std::list<Folder*>& pfolder_list) {
  for (auto pfolder : pfolder_list) {
    AddFolder(pfolder);
  }
}

void Message::BatchRemove() {
  for (auto pfolder : pfolder_list_) {
    RemoveFolder(pfolder);
  }
}

} // namespace cp
