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
需要反思的有：
1. 在使用g++编译str_vec.cc的时候，可以直接看到没有符号，所以不用再使用make了。
2. 或许有了猜想，可以先试探看看。不见得确定，但是试一下总是可以的。

在总结一个坑，针对pimpl-idiom，涉及到对于copy semantics的理解：
1. demo-01的实践非常清晰，strvec->dynamic memory，所以copy semantics涉及到不同对象的alloc成员(简单理解为指针即可)
2. demo-02变成了这样，strvec->strvec_impl->dynamic memory，所以，此时多了一层。每一层都需要考虑copy semantics。
2.1. 对于strvec_impl我禁止了copy(这个做的是正确的，但我也是后知后觉才发现)，strvec_impl作为资源句柄类，介于strvec与dynamic memory之间，应该是每个strvec所独有。
2.2. 对于strvec，本身的语义支持copy，并且是valuelike属性，但是，这里的valuelike是指dynamic memory，对于pimpl这个成员，不进行拷贝，从成员的角度来说，比较难理解，但是从整体的拷贝语义来看，是没问题的。
2.3. 所以，对于strvec，支持valuelike copy semantics，但是资源管理句柄不进行拷贝，实现value like copy semantics的是pimpl管理的dynamic memory.
