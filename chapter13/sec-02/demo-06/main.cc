#include "tree.h"

using namespace cp;

int main(void) {
  std::shared_ptr<TreeNode> father_ptr = std::make_shared<TreeNode>("A");
  std::shared_ptr<TreeNode> lchild_ptr = std::make_shared<TreeNode>("B");
  std::shared_ptr<TreeNode> rchild_ptr = std::make_shared<TreeNode>("C");

  father_ptr->SetLeftChild(lchild_ptr);
  father_ptr->SetRightChild(rchild_ptr);

  std::shared_ptr<Tree> root_ptr = std::make_shared<Tree>(father_ptr);

  return 0;
}
