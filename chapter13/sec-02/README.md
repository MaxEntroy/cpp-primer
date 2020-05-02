[TOC]

## Section13.2 Copy Control and Resource Management

### 实践

- demo-01

自己实现了value-like版本

- demo-02

根据书上的要求，修正了demo-01的问题，主要是两个方面
1. 自赋值的问题需要修复
2. 析构left operand和复制right operand的顺序

- demo-03

自己实现了pointer-like版本，这里和书上的建议实现一致，采用smart pointer管理需要共享语义的资源

- demo-04

自己实现了pointer-like不借助shared_ptr的方式，跟书上的做法有一些区别
1. ref和data作为逻辑整体
2. thread-safe
3. 但是目前还是存在bug, thread-safe对于拷贝没有问题，因为拷贝只涉及到增加ref，但是赋值还牵扯到析构

```cpp
HasPtr& HasPtr::operator=(const HasPtr& rhs) {
  if (this == &rhs) {
    return *this;
  }

  auto ptmp = rhs.pstr_;
  if (ptmp) {
    ptmp->IncrRef();
  }

  pstr_->DecrRef();
  // here is the problem
  if (pstr_->GetRef() == 0) {
    delete pstr_;
  }

  pstr_ = ptmp;
  return *this;
}
```

如上，我们看到如果对于引用计数的自减操作和获取操作以及析构操作不是一个原子操作，会存在问题。
比如，对象1和对象2同时指向一个数据，此时对象1先析构，正常来说只会对引用计数做自减，但是如果此时程序调度，对象2也开始析构，那么对象2会完成剩余操作。此时再回到对象1，引用计数为0，尝试析构，但此时动态数据已经被析构。导致undefine behavior.