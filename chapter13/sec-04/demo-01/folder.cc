#include "folder.h"

namespace cp {

void Folder::AddMsg(Message* pmsg) {
  pmsg_list_.push_back(pmsg);
}

void Folder::RemoveMsg(Message* pmsg) {
  pmsg_list_.remove(pmsg);
}

} // namespace cp
