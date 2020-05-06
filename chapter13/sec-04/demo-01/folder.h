#ifndef FOLDER_H_
#define FOLDER_H_

#include <list>

namespace cp {

class Message;

class Folder {
 public:
  void AddMsg(Message*);
  void RemoveMsg(Message*);
 private:
  std::list<Message*> pmsg_list_;

};

} // namespace cp

#endif // FOLDER_H_
