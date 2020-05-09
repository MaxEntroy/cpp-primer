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
2. move semantics
3. copy semantics
