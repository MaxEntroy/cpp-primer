#include "binode.h"

namespace cp {

BiNode::~BiNode() {
  ReleaseChild();
}

BiNode::BiNode(const BiNode& rhs) {
  val_ = rhs.val_;
  ref_cnt_ = rhs.ref_cnt_;
  CopyChild(rhs);
}

BiNode& BiNode::operator=(const BiNode& rhs) {
  if (this == &rhs) {
    return *this;
  }

  ReleaseChild();

  val_ = rhs.val_;
  ref_cnt_ = rhs.ref_cnt_;
  CopyChild(rhs);

  return *this;
}

void BiNode::CopyChild(const BiNode& rhs) {
  auto pl = rhs.pleft_;
  if (pl) {
    pl->ref_cnt_++;
  }
  auto pr = rhs.pright_;
  if (pr) {
    pr->ref_cnt_++;
  }

  pleft_ = pl;
  pright_ = pr;
}

void BiNode::ReleaseChild() {
  if (pleft_) {
    pleft_->ref_cnt_--;
    if (!(pleft_->ref_cnt_)) {
      delete pleft_;
    }
  }

  if (pright_) {
    pright_->ref_cnt_--;
    if (!(pright_->ref_cnt_)) {
      delete pright_;
    }
  }
}

} // namespace cp
