#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <iostream>
#include <list>
#include <string>
#include <utility>

#include "folder.h"

namespace cp {

class Message {
 public:
  explicit Message(const std::string& s) : msg_(s) {}

  ~Message();
  Message(const Message&);
  Message& operator=(const Message&);

  void AddFolder(Folder*);
  void RemoveFolder(Folder*);

  void PrintMsg() const {
    std::cout << msg_ << std::endl;
  }

  friend void swap(Message&, Message&);
 private:
  void BatchAdd(const std::list<Folder*>&);
  void BatchRemove();

 private:
  std::string msg_;
  std::list<Folder*> pfolder_list_;
};

inline
void swap(Message& lhs, Message& rhs) {
  using std::swap;
  swap(lhs.msg_, rhs.msg_);

  for (auto pfolder : lhs.pfolder_list_) {
    pfolder->RemoveMsg(&lhs);
  }

  for (auto pfolder : rhs.pfolder_list_) {
    pfolder->RemoveMsg(&rhs);
  }

  swap(lhs.pfolder_list_, rhs.pfolder_list_);
  for (auto pfolder : lhs.pfolder_list_) {
    pfolder->AddMsg(&lhs);
  }
  for (auto pfolder : rhs.pfolder_list_) {
    pfolder->AddMsg(&rhs);
  }
}

} // namespace cp

#endif // MESSAGE_H_
