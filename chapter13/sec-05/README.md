[TOC]

## Section13.5. Classes That Manage Dynamic Memory

### 实践

- demo-01

看过一遍书，有初步的认识，然后考虑自己的实现。基本的一个设计如下：

- 分配策略(low-level memory management)
  - 2倍预分配/allocator
  - reallocation/check
  - first/last/end
- copy-control
  - ctor
    - VecStr a
    - VecStr b = {"hello", "world"}
  - dtor
  - ctor
  - optr=
- implementation
  - typedef std::string* striter
  - create()/create(striter b, striter e)
  - uncreate()

- demo-02

尝试优化以下几个点：
1. pimpl-idiom
2. copy semantics
2.1. copy constructor/copy assignment正确顺序
2.2. move operations

具体的实现的时候，发现
1. pimpl-idiom的实现，明显难度加大，对于指针布局的理解需要更深入的理解。但是，不管理解是否深入，需要做到的就是语义清晰. incr_last()这个操作理解要准确
