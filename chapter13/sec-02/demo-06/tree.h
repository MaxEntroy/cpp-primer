#ifndef TREE_H_
#define TREE_H_

#include "treenode.h"

namespace cp {

class Tree {
  std::shared_ptr<TreeNode> root_;

 public:
  explicit Tree(const std::shared_ptr<TreeNode>& ptr) : root_(ptr) {}
};

} // namespace cp

#endif // TREE_H_
