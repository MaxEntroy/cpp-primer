[TOC]
## Section12.1 Dynamic Memory and Smart Pointers

q:什么是dynamic memory?还有其他何种类型的memory?
- Static memory is used for local static objects, for class static data members,and for variables defined outside any function
- Stack memory is used for nonstatic objects defined inside functions
- Dynamic memory is used for dynamic objects.

q:区别以上3种memory的本质条件是什么？
>本质是从对象的生存期来考察的。即，分配到以上三种内存的objects，就决定了其各自的lifetime.

- Static objects are allocated before they are used, and they are destroyed when the program ends.
- Stack objects exist only while the block in which they are defined is executing
- Programs use the heap for objects that they dynamically allocate—that is, for objects that the program allocates at run time.

当然，这里需要特别注意的是：
- Objects allocated in static or stack memory are automatically created and destroyed by the compiler
- The program controls the lifetime of dynamic objects

以上是从语言层面做的解读，落地到不同的平台(os)，有其各自大同小异的实现。

### 12.1.1. The shared_ptr Class

#### basis

这里也没设么太多好总结的，ec当中已经说得很清楚了。cpp通过RAII来进行资源管理，对于内存这种资源，则可以使用库定义好的smart pointers object来进行管理。所以，对于programmer，就是学习这种RAII对象的使用方式即可。

#### The make_shared Function

q:makes_shared的作用是什么?
>The safest way to allocate and use dynamic memory is to call a library function named make_shared. This function allocates and initializes an object in dynamic memory and returns a shared_ptr that points to that object.
>
>这么理解，如果我们直接管理内存。需要使用new和delete进行资源的申请(分配-初始化)和释放(析构-释放)。但是对于shared_ptr，也是用raii的方式进行管理，make_shared相当于资源申请的工厂方法，raii的核心思路，资源申请的同时进行资源管理对象的初始化。

#### Copying and Assigning shared_ptrs

q:shared_ptr的语义是什么?
>Smart pointer that provides shared ownership, which allows multiple pointers to refer to the same object

#### shared_ptrs Automatically Destroy Their Objects and Automatically Free the Associated Memory

#### Classes with Resources That Have Dynamic Lifetime

q:什么时候考虑使用dynamic memory?
- They don’t know how many objects they’ll need
- They don’t know the precise type of the objects they need 
- They want to share data between several objects

#### Defining the StrBlob Class

q:如何设计一个collection of string的类型，要求该类型采用共享语义，即底层数据共享？
>作者的设计思路非常的清晰，这个是我们能学到的。整体原则就一个，多用基础组件，避免自己造轮子。
>
>首先，这肯定是一个sequential string container，那么我们如何组织。按照整体原则，考虑库的情形，vector<string>是首选。
>
>其次，共享语义如何实现。如果将vector<string>作为类的成员，那么vector<string>的life time也就是class object的life time.加入把一个对象赋值给另一个对象，考虑vector<string>的assigning语义，完成深拷贝。即，共享语义没有实现。
从另一个角度说，即使是浅拷贝。那么一个对象析构，无论如何vector<string>也会被回收。另外一个对象还是无法完成共享语义。
>答案就是，把vector<string>分配到dynamic memory，我们自己来控制它的lifetime.
那么紧接而来又是另一个问题，如何管理？按着整体的设计原则，尽量使用基础库。我们考虑smart pointer。那么我们该使用哪一个呢？
显然，该使用shared_ptr，因为共享语义。
>
>至此，类的数据我们已经设计完毕，利用shared_ptr来对vector<string>进行管理。

#### StrBlob Constructors

#### Element Access Members

#### Copying, Assigning, and Destroying StrBlobs

q:当我们讨论一个类的copying,assigning,destroying时，我们在讨论什么?
>我们在讨论其成员的，copying,assigning and destroying.
>
>shared_ptr<vector<string>> data;这个成员的copying, assigning, destroying的语义，符合我们本例的讨论。
所以，不用额外的操作

### 实践

- demo-01

这个case给出了对于StrBlob的基本实现，需要注意的是对于单元测试的一些写法.

q:private methods如何进行unit test?
>主要是参考了google gtest给出的建议。我在demo-01采用了FRIEND_TEST的方法

- 侵入被测试类，声明FRIEND_TEST
- 被测试类需要link gtest
- 测试类需要采用和被测试类一样的名字空间

[Advanced googletest Topics](https://github.com/google/googletest/blob/master/googletest/docs/advanced.md)

q:如何对于thow的函数进行测试？有什么问题？
>同上，参考google gtest给出的断言。
>
>问题在于，不能测试exception message，这是一个看起来可能很严重的失误，只用断言判断异常类型不足以解决异常类型一直但异常信息不一致的情形