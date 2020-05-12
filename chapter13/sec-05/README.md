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
2. 修正一下1中的理解，这么理解是错误的

说一下编译时候碰到的问题：
1. 由于str_vec.h采用了forward declaration，所以不能定义inline function.
2. 我尝试在str_vec.cc当中定义inline函数，因为此时已经看得到定义。
3. 1和2的最初思考，导致了编译的问题。简单理解为，inline声明，和在str_vec.h当中声明是一样的，此时forward declaration，不能使用函数定义去执行操作。
4. 所以，如果使用了forward declaration，和这个类相关的inline function不能使用。

说一下整体的排查思路：
1. 最初是看到.a里面没有符号，用g++直接编译了下，发现对不上，怀疑是blade的问题
2. 采用make进行编译，发现也过不去
3. 1和2证明不是编译工具的问题，应该是自己代码的问题。此时的猜测，要么inline不对，要么forward declaration不对。
4. 验证表明Inline不对，google一下验证猜想.

需要反思的有：
1. 在使用g++编译str_vec.cc的时候，可以直接看到没有符号，所以不用再使用make了。
2. 或许有了猜想，可以先试探看看。不见得确定，但是试一下总是可以的。

