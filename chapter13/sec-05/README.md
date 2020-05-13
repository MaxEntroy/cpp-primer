[TOC]

## Section13.5. Classes That Manage Dynamic Memory

### Basis

q:使用dynamic memory的动机？
>这一点在smart pointer的章节已经进行过说明，本小节使用的原因：
Some classes need to allocate a varying amount of storage at run time.
建议的做法，使用诸如vector这样的library来解决，一来可以避免自己控制，二来vector控制的不容易出错(自己更容易出错)
但是，如果需要比较细粒度的控制，只能是自己来。比如，本例中reallocate使用的move operation

### Moving, Not Copying, Elements during Reallocation

这一小节的内容比较新，所以单独说下。

q:作者在这里的考虑是什么?
>Before we write the reallocate member, we should think a bit about what it must
do. This function will
- Allocate memory for a new, larger array of strings
- Construct the first part of that space to hold the existing elements
- Destroy the elements in the existing memory and deallocate that memory

这里有一个特别容易理解错的地方：
1.使用move operation不是为了避免以上的开销，以上的开销无论如何都是要有的。
2.以上开辟的空间，每一个元素是一个std::string，但是std::string其实也是个句柄类，它管理了一块dynamic memory
3.所以，move operation真正优化的是，对于这块dynamic memory的开销

之所以这么做的原因是：
1.std::string是valuelike copy semantics
2.拷贝对象在拷贝完成后，马上就析构了。我们考虑拷贝完成，但尚未析构的这个时刻
2.1. 拷贝对象管理一块dynamic memory
2.2. 被拷贝对象也管理一块dynamic memory
2.3. 而2.1中的内容马上要被析构
3.所以，对于即将要被析构的对象，没有必要再进行拷贝，我们可以moving resources from the given object to theobject being constructed.

简单总结下，使用move operation的场景：
1.肯定需要有拷贝操作，这个无需多言
2.拷贝对象是valuelike copy semantics，这点特别重要，如果不是因为这一点，那么是否使用move operation没有关系
3.只有在拷贝发生在，拷贝对象即将析构时，此时使用move operation来移动资源。
4.被移动资源的拷贝对象，可以正常析构
5.move operation本质来说，比较类似shalow copy，即通过简单的指针复制，来获取拷贝对象的资源，但同时保证了该对象可以正常析构

### Move Constructors and std::move

q:move constructor的semantics?
>本质来说，也是copy semantics，但是可以细分说成是move semantics

一些细节：
1. However, we do know that move
constructors typically operate by “moving” resources from the given object to the
object being constructed
2. We also know that the library guarantees that the “movedfrom”
string remains in a valid, destructible state

q:怎么样触发move semantics?
>我们上面说了，move semantics本质是copy semantics，所以直接的拷贝操作不会触发move constructor
具体是，在对象进行拷贝的时候，调用std::move

q:std::move的作用是什么?
>在发生拷贝时，通过调用std::move来触发move constructor

q:std::move有什么需要注意的？
>When we use move, we call std::move, not move

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
2.1. copy constructor/copy assignment正确顺序(这个没有实现，原因在于我看课本的做法也没有判断先申请空间是否成功，所以如果是为了避免自赋值，这个我解决了。)
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

- demo-03

补充课后习题的问题 

- 重新实现了realloc_mem，来支持reserve/resizes以及push_back逻辑，没有必要重载
- 调整了类型
- 没有采用for_each的实现，参数传递语义不清晰，我不需要传值，但是需要通过引用传值，在获取迭代器