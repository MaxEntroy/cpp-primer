#ifndef BITREE_H_
#define BITREE_H_

#include "binode.h"

namespace cp {

class BiTree {
 public:
  explicit BiTree(BiNode* p) : proot_(p) {}

  ~BiTree();
  BiTree(const BiTree&);
  BiTree& operator=(const BiTree&);

 private:
  BiNode* proot_;
};

} // namespace cp

#endif // BITREE_H_
