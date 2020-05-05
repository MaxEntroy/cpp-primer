#ifndef TREENODE_H_
#define TREENODE_H_

#include <memory>
#include <string>

namespace cp {

class TreeNode {
  std::string val_;
  std::shared_ptr<TreeNode> l_;
  std::shared_ptr<TreeNode> r_;

 public:
  explicit TreeNode(const std::string& v) : val_(v) {}

  void SetLeftChild(const std::shared_ptr<TreeNode>& ptr) {l_ = ptr;}
  void SetRightChild(const std::shared_ptr<TreeNode>& ptr) {r_ = ptr;}
};

} // namespace cp

#endif // TREENODE_H_
