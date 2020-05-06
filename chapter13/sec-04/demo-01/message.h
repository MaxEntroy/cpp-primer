#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <list>
#include <string>

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

 private:
  void BatchAdd(const std::list<Folder*>&);
  void BatchRemove();

 private:
  std::string msg_;
  std::list<Folder*> pfolder_list_;
};

} // namespace cp

#endif // MESSAGE_H_
