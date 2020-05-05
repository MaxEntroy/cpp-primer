#ifndef BINODE_H_
#define BINODE_H_

#include <string>

namespace cp {

class BiNode {
 public:
  explicit BiNode(const std::string& v) : val_(v), ref_cnt_(1), pleft_(nullptr), pright_(nullptr) {}

  ~BiNode();
  BiNode(const BiNode&);
  BiNode& operator=(const BiNode&);

  void SetLeft(BiNode* p) { pleft_ = p; }
  void SetRight(BiNode* p) { pright_ = p; }

 private:
  void CopyChild(const BiNode&);
  void ReleaseChild();

 private:
  std::string val_;
  int ref_cnt_;
  BiNode* pleft_;
  BiNode* pright_;
};


} // namespace cp

#endif // BINODE_H_
