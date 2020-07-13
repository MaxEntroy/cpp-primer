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

q:为什么建议使用make_shared来构造shared_ptr而不是直接调用shared_ptr constructors with new?
>这里面有性能上的考虑
In a typical implementation, shared_ptr holds only two pointers:
- the stored pointer (one returned by get());
- a pointer to control block.

The control block is a dynamically-allocated object that holds:
- either a pointer to the managed object or the managed object itself;
- the deleter (type-erased);
- the allocator (type-erased);
- the number of shared_ptrs that own the managed object;
- the number of weak_ptrs that refer to the managed object.

>When shared_ptr is created by calling std::make_shared or std::allocate_shared, the memory for both the control block and the managed object is created with a single allocation.The managed object is constructed in-place in a data member of the control block
>
>When shared_ptr is created via one of the shared_ptr constructors, the managed object and the control block must be allocated separately. In this case, the control block stores a pointer to the managed object.

q:make_shared有什么缺点吗?
>这个问题需要结合weak_ptr来一起看，我在weak_ptr对这个问题进行解决。

参考
[Difference in make_shared and normal shared_ptr in C++](https://stackoverflow.com/questions/20895648/difference-in-make-shared-and-normal-shared-ptr-in-c)

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
>其次，共享语义如何实现。如果将vector<string>作为类的成员，那么vector<string>的life time也就是class object的life time.假如把一个对象赋值给另一个对象，考虑vector<string>的assigning语义，完成深拷贝。即，共享语义没有实现。
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

### 12.1.2. Managing memory directly

本小节对于new和delete的一些细节进行讨论，根本目的还是告诉我们尽量使用smart pointer.

**caution: Managing Dynamic Memory Is Error-Prone**<br>
There are three common problems with using new and delete to manage dynamic memory:
- Forgetting to delete memory
- Using an object after it has been deleted.
- Deleting the same memory twice

**Best Practices: You can avoid all of these problems by using smart pointers exclusively.**

#### Using new to Dynamically Allocate and Initialize Objects

q:什么是value initialization?和dafault initialization有什么区别?
>We can usually omit the value and supply only a size. In this case the library creates a value-initialized element initializer for us.
>
>前者对于built-in type，不会使用任意值，比如int则会使用0进行初始化。class type则是default constructor。而后者对于
built-in type则是使用任意值进行初始化，而class type则是default constructor.

#### Dynamically Allocated const Objects

#### Memory Exhaustion

q:new (nothrow) int 和 new int有什么区别？
>对于后者，if allocation fails, new throws std::bad_alloc
对于前者，if allocation fails, new returns a null pointer

#### Freeing Dynamic Memory

q:delete执行什么操作?
>Like new, a delete expression performs two actions: It destroys the object to which
its given pointer points, and it frees the corresponding memory.

#### Pointer Values and delete

q:对于delete而言，什么行为会触发undefined behavior?
- Deleting a pointer to memory that was not allocated by new(即释放非动态类型内存)
- deleting the same pointer value more than once(同一块动态内存释放两次)

#### Dynamically Allocated Objects Exist until They Are Freed

#### Resetting the Value of a Pointer after a delete...

q:dangling pointer是什么意思？这会导致什么问题?应该如何解决?
>A dangling pointer is one that refers to memory that once held an object but no longer does so.
即释放动态内存后的指针，但是该指针还保留着之前动态内存的地址。由于这块内存已经被释放了，所以如果继续访问会出问题，因为
这块内存现在保留的可能是任意值(垃圾值)，当然也有可能这块内存又被分配给别人了。总是如果使用dangling pointer所指向的内存，
会有可能触发hidden bugs.
>
>解决办法就是对dangling pointer赋予nullptr.

#### ...Provides Only Limited Protection

我们看这样一段代码
```cpp
int *p(new int(42)); // p points to dynamic memory 
auto q = p; // p and q pointtothesamememory 
delete p; // invalidates both p and q
p = nullptr; // indicates that p is no longer bound to an object
```

q:上面这段代码想反映什么问题?
>A fundamental problem with dynamic memory is that there can be several pointers that point to the same memory.
>
>如果我们想用new/delete来管理动态内存，那么很有可能有多个指针指向同一块动态内存。因为指针之间的copy,assign是可能的。
此时，如果我们通过某一个指针释放了这块动态内存，并且解决了dangling pointer，但是难得是我们很难知道还有哪些指针也同样指向这块内存。如果那些指针不能对dangling pointer进行操作，一样会碰到现在的问题。

### 12.1.3 Using shared_ptrs with new

看下面这段代码

```cpp
shared_ptr<int> p1 = new int(1024); // error: must use direct initialization
shared_ptr<int> p2(new int(1024)); // ok: uses direct initialization
```

q:smard pointer是否接收普通指针的隐式转换?
>No.The smart pointer constructors that take pointers are explicit (§ 7.5.4, p. 296). Hence, we cannot implicitly convert a built-in pointer to a smart pointer

**我们可以发现，using shared_ptr with new 是一种raii的直接体现**

#### Don’t Mix Ordinary Pointers and Smart Pointers ...

这一小节我觉得还挺重要，主要讲了一些常见的错误。尤其是指针混用的问题。确实容易出现

看下面代码进行讨论:

```cpp
void process(shared_ptr<int> ptr)
{
// use ptr
} // ptr goes out of scope and is destroyed

shared_ptr<int> p(new int(42)); // reference count is 1
process(p); // copying p increments its count; in process the reference count is 2
int i = *p; // ok: reference count is 1

int *x(new int(1024)); // dangerous: x is a plain pointer, not a smart pointer
process(x); // error: cannot convert int* to shared_ptr<int> 
process(shared_ptr<int>(x)); // legal, but the memory will be deleted!
int j = *x; // undefined: x is a dangling pointer!
```

**Best practice: shared_ptr和make_shared联合使用。尽量不使用shared_ptr和new的组合**

**Warning: It is dangerous to use a built-in pointer to access an object owned by a smart pointer, because we may not know when that object is destroyed.**

#### ...and Don’t Use get to Initialize or Assign Another Smart Pointer

q:smart pointers提供的get方法的正确用法是什么?
>This function is intended for cases when we need to pass a built-in pointer to code that can’t use a smart pointer.
>
>比如lua capi，如果我们在cpp中用raii封装LuaState，但是此时调用api时都是capi，不支持smart pointers.

**Warning：Although the compiler will not complain, it is an error to bind another smart pointer to the pointer returned by get. 因为这会导致两个独立的smart pointer指向同一块资源，千万注意，是两个独立的smart pointer，这么做不会形成共享语义**

看下面代码

```cpp
shared_ptr<int> p(new int(42)); // reference count is 1
int *q = p.get(); // ok: but don't use q in any way that might delete its pointer
{ // new block
// undefined: two independent shared_ptrs point to the same memory 
shared_ptr<int>(q);
} // block ends, q is destroyed, and the memory to which q points is freed
int foo = *p; // undefined; the memory to which p points was freed
```

这里需要注意的是，```shared_ptr<int>(q)```和```shared_ptr<int> p(new int(42))```不是同一个smart pointer，因为前者不是通过后者进行的copy and assign，所以没有reference count的变化。这二者是独立的smart pointer.两个独立的smart pointer
指向同一块资源，肯定会导致灾难。而这种情况，只有通过warning中禁止的行为才会发生。

#### Other shared_ptr Operations

q:reset会更新reference count嘛?
>Like assignment, reset updates the reference counts and, if appropriate, deletes the object to which p points

q:reset的正确用法是什么?
>The reset member is often used together with unique to control changes to the object shared among several shared_ptrs. 
Before changing the underlying object, we check whether we’re the only user. If not, we make a new copy before making the change

```cpp
if (!p.unique())
  p.reset(new string(*p)); // we aren't alone; allocate a new copy
*p += newVal;
```

#### A short summary

这一小节其实比较重要，因为阐述了智能指针和原始指针的关系。在一些用法上要特别注意，因为极容易出错。我来简单总结下。

- 接口提供ordinary pointer
  - dynamic object采用ordinary pointer管理。这一种没什么好说的，原始c的方式。
  - dynamic object采用smart pointer管理。
    - 正确的用法是对于CAPI，调用get()方法即可
    - 但是调用get()方法之后，务必不能根据ordinary pointer再构造一个smart_pointer
- 接口提供smart pointer
  - dynamic object采用ordinary pointer管理。Not Recommended!!!
    - 这种case非常容易出错。
    - 建议改变接口参数，或者资源管理的方式
  - dynamic object采用smart pointer。c++建议方式

参考demo-05的实践

### 12.1.4. Smart Pointers and Exceptions

这一小节也是比较实用的章节，主要讲了smart pointer(shared_ptr)在管理dynamic memory以外资源的一些用法。os当中除了memory
还有很多常用资源
- FILE(FILE or fd)
- socket/port/connection
- db(mysql/redis)
- 其他非Os资源,lua state

q:smart pointers对于异常导致的程序执行流程以外变化而导致的resources leak是否可以避免?
>if an exception occurs. One easy way to make sure resources are freed is to use smart pointers.

#### Smart Pointers and Dumb Classes

q:dumb classes是指什么?
>Many C++ classes, including all the library classes, define destructors (§ 12.1.1, p. 452) that take care of cleaning up the resources used by that object.
>
>However, not all classes are so well behaved.
>
>In particular, classes that are designed to be used by both C and C++ generally require the user to specifically free any resources that are used
>
>我自己的理解是，需要自己手动管理资源的类，叫做dumb classes.导致这个类产生的最常见的问题在于，c++对于c的兼容，由于c不支持class机制，所以一些library的实现考虑到了c的场景。此时，需要我们自己来管理这些资源。
一个显而易见的例子就是在c++当中调用lua时，不可避免的会用到liblua.a，而这个库是用c写成的。所以，对于lua state这种资源，
我们需要通过raii的方式进行管理。
>而smart pointer存在的意义在于，他为我们提供了灵活的接口，让我们不必要自己重新实现一个raii class，直接借助smart pointer即可进行资源管理

#### Using Our Own Deletion Code

作者给出了一个例子
```cpp
struct destination; // represents what we are connecting to
struct connection; // information needed to use the connection connection connect(destination*); // open the connection 
void disconnect(connection); // close the given connection void f(destination &d /* other parameters */)
{
// get a connection; must remember to close it when done
  connection c = connect(&d);
// use the connection

// if we forget to call disconnect before exiting f, there will be no way to close c
  disconnect(c)
}
```
对于上述代码，我们面临的问题是，如何管理connection这个资源，使得资源泄露的问题可以避免。
**显然可以通过raii的手法来进行资源管理**，即需要定义资源管理类。

1. 自己定义一个raii类
2. 借助现有工具

我们先考虑第一个方法，大致的实现可能如下
```cpp
class ConnectionType {
 public:
  ConnectionType(destination* p_des) : p_conn(nullptr) {
    *p_conn = connconnect(p_des)
  }
  ~ConnectionType() {disconnect(*p_conn;)}
  connection* get() const {return p_conn;}

 private:
  connection* p_conn;
};

void process(destionation* p_des) {
  ConnectionType conn(p_des);

  do_something(conn.get())
}
```

但是，上面代码没有考虑复用的角度。既然smart pointer给我们提供了工具，那我们应该尝试用smart pointer来解决这个问题
1. By default, shared_ptrs assume that they point to dynamic memory. 所以，如果我们希望管理connection资源，shared_ptr内部的plain pointer不能指向dynamic memory，而是应该指向connection.
2. default, when a shared_ptr is destroyed, it executes delete on the pointer it holds. 同理，如果我们希望回收connection资源，我们也需要重新定义deleter函数，来处理内部的plain pointer

```cpp
void end_connection(connection* p_conn) {disconnect(*p_conn);}
// disconnect不能作为deleter的原因在于，deleter的参数，必须是T*，因为shared_ptr在实现的时候，回调接口就是这么设计的

connection conn = connect(&des);
shared_ptr<connection> conn_ptr(&conn, end_connection);
// conn不能作为shared_ptr的参数原因是，shared_ptr的构造接口参数必须是T*，因为他的内部是通过plain pointer对资源进行管理的
```
**Caution: Smart Pointer Pitfalls**

q:使用smart pointer有哪些注意点，能保证我们正确使用？
>Smart pointers can provide safety and convenience for handling dynamically allocated memory only when they are used properly. To use smart pointers correctly, we must adhere to a set of conventions
- Don’t use the same built-in pointer value to initialize (or reset) more than one smart pointer
- Don’t delete the pointer returned from get().
- Don’t use get() to initialize or reset another smart pointer.
- If you use a pointer returned by get(), remember that the pointer will become invalid when the last corresponding smart pointer goes away.
- If you use a smart pointer to manage a resource other than memory allocated by new, remember to pass a deleter

### 12.1.5 unique_ptr

#### basic
q:unique_ptr的语义是什么?和shared_ptr的语义有什么区别?
>std::unique_ptr is a smart pointer type introduced in C++11, which expresses exclusive ownership of a dynamically allocated object.
It cannot be copied, but can be moved to represent ownership transfer.
>
> std::shared_ptr is a smart pointer type that expresses shared ownership of a dynamically allocated object.

参考
[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html#Ownership_and_Smart_Pointers)
[聊聊 C++ 的优雅写法](https://bot-man-jl.github.io/articles/?post=2020/Conventional-Cpp#smart-pointers-ownership)

q:unique_ptr如何进行构造?
>Unlike shared_ptr, there is no library function comparable to make_shared that returns a unique_ptr. Instead, when we define a unique_ptr, we bind it to a pointer returned by new.

q:release是否析构对象?
>Calling release breaks the connection between a unique_ptr and the object it had been managing.
>Often the pointer returned by release is used to initialize or assign another smart pointer. In that case, responsibility for managing the memory is simply transferred from one smart pointer to another. However, if we do not use another smart pointer to hold the pointer returned from release, our program takes over responsibility for freeing that resource:

#### Passing and Returning unique_ptrs

q:有什么特例可以让我们进行copy or assign a unique_ptr?
>There is one exception to the rule that we cannot copy a unique_ptr: We can copy or assign a unique_ptr that is about to be destroyed.The most common example is when we return a unique_ptr from a function

#### Passing a Deleter to unique_ptr

q:unique_ptr在passing a deleter时有什么特别之处?
>we must supply the deleter type inside the angle brackets along with the type to which the unique_ptr can point.We supply a callable object of the specified type when we create or reset an object of this type

看书上的demo，我来解释下参数类型
- decltype(end_function)，给出end_function的类型
- 但是，第二个参数这里填的是一个callable object的类型，对于c当中的callable object，其实就一种。function pointer type
- 注意，function pointer type不是function type，这里要做下严格区别，起码decltype不会从end_connection推导出void (*)(connection*)这样的类型

```cpp
void f(destination &d /* other needed parameters */)
{
connection c = connect(&d); // open the connection // when p is destroyed, the connection will be closed
unique_ptr<connection, decltype(end_connection)*> p(&c, end_connection);
// use the connection
// when f exits, even if by an exception, the connection will be properly closed }
```

### 12.1.6 weak_ptr

#### basic

首先，我们先来回答一个之前的问题

q:make_shared有什么缺点吗?
>我们先复习shared_ptr的实现细节
In a typical implementation, shared_ptr holds only two pointers:
- the stored pointer (one returned by get());
- a pointer to control block.
The control block is a dynamically-allocated object that holds:
- either a pointer to the managed object or the managed object itself;
- the deleter (type-erased);
- the allocator (type-erased);
- the number of shared_ptrs that own the managed object;
- the number of weak_ptrs that refer to the managed object.

需要特别说明的是，the stored pointer和a pointer to control block不一定是相等的。

我们需要进一步关注的是，shared_ptr的析构行为是怎样的?
>The destructor of shared_ptr decrements the number of shared owners of the control block. If that counter reaches zero, the control block calls the destructor of the managed object. The control block does not deallocate itself until the std::weak_ptr counter reaches zero as well.
>
>从control block的成员来看，the number of shared_ptr和the number of weak_ptr不是同一个成员，这也就说明了
不同的行为，会更新不同的变量。比如，shared_ptr之间的copy,assign一般会增加the number of shared_ptr(当然也有可能增加
the number of weak_ptr)，但是，weak_ptr通过shared_ptr进行初始化，或者weak_ptr之间的copy,assign并不会增加the number of shared_ptr，而是增加the number of weak_ptr.
所以，这也到control block存在两次析构，一次是当shared_ptr的最后一个对象析构，那么control block会析构the managed object，但是不会析构control block。只有当weak_ptr的最后一个对象析构时，control block才会析构。
>
>这会出现一种合理的情形是，如果一个weak_ptr所指向的shared_ptr都析构了，但是weak_ptr还未析构的情形。
此时，如果我们采用make_shared进行构造，由于后者采用only one allocation，所以即使the managed object可以析构，
但是也不能析构，需要等待weak_ptr引用计数为0，control block进行析构，才能一起析构。
显然，如果采用shared_ptr(constructor)则不会出现这种case.

参考
[Difference in make_shared and normal shared_ptr in C++](https://stackoverflow.com/questions/20895648/difference-in-make-shared-and-normal-shared-ptr-in-c)
[std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr)

q:weak_ptr的语义是什么?
>std::unique_ptr is a smart pointer type introduced in C++11, which expresses exclusive ownership of a dynamically allocated object.
std::shared_ptr is a smart pointer type that expresses shared ownership of a dynamically allocated object.
weak_ptr captures the idea that a weak_ptr shares its object “weakly.
>
>我们来看一些weak_ptr的细节：
- A weak_ptr (Table 12.5) is a smart pointer that does not control the lifetime of the object to which it points.
- a weak_ptr points to an object that is managed by a shared_ptr
- Binding a weak_ptr to a shared_ptr does not change the reference count of that shared_ptr
- Once the last shared_ptr pointing to the object goes away, the object itself will be deleted. That object will be deleted even if there are weak_ptrs pointing to it
>通过我们上文对shared_ptr内部结构的分析，我们知道，当shared_ptr的rc为0是，管理的对象会被析构，但是control block不一定会被析构。只有当weak_ptr的rc为0时，control block才会被析构

1. 这上面总结的第一点其实非常重要，当shared_ptr形成cycle references时，至于哪一个shared_ptr该采用weak_ptr则完全根据第一点来判断。
2. 第二点告诉我们，weak_ptr的初始化必须通过shared_ptr来进行。

q:weak_ptr使用时，需要注意什么?
>Because the object might no longer exist, we cannot use a weak_ptr to access its object directly.To access that object, we must call lock.

#### destructor of shared_ptr and weak_ptr

这是非常重要的一小节，其实我在上面讲述make_shared存在的缺点时，已经介绍了shared_ptr的析构过程。但是，由于这一小节非常重要，所以我们再次进行讨论。因为有很多需要特别在意的地方。

1. 务必区分smart_ptr的析构，和managed object的析构，这两个不是一回事，不要搞混了。否则shared_ptr形成cycle reference时不能造成彼此析构逇解释，很容易说成和死锁一样，不是这么一回事，真正的原因是，smart pointer已经析构了，但是managed object没有析构。
2. shared_ptr的析构过程是怎样的？这个一定要结合shared_ptr的结构来说，由于shared_ptr是共享了managed object，所以一个shared_ptr的析构，不可能简单的对于managed object进行析构。<br>
2.1. The destructor of shared_ptr decrements the number of shared owners of the control block. If that counter reaches zero, the control block calls the destructor of the managed object. The control block does not deallocate itself until the std::weak_ptr counter reaches zero as well<br>
2.2. In a typical implementation, shared_ptr holds only two pointers:<br>
2.2.1. the stored pointer (one returned by get());<br>
2.2.2. a pointer to control block.<br>
2.3. The control block is a dynamically-allocated object that holds:<br>
2.3.1. either a pointer to the managed object or the managed object itself;<br>
2.3.2. the deleter (type-erased);<br>
2.3.3. the allocator (type-erased);<br>
2.3.4. the number of shared_ptrs that own the managed object;<br>
2.3.5. the number of weak_ptrs that refer to the managed object.<br>
2.4. 具体到weak_ptr的析构，由于weak_ptr的构造，并不增加shared_ptr's rc，只是增加weak_ptr's rc，所以，析构的时候反之亦成。只是减少weak_ptr's rc，如果rc==0，那么此时会析构control block对象。<>
3. In existing implementations, the number of weak pointers is incremented ([1], [2]) if there is a shared pointer to the same control block. 这句话也非常重要，shared_ptr进行copy,assign时，除了增加the number of shared_ptrs，也增加nunmber weak_ptrs.同理，shared_ptr进行析构时，也是同时更新两个rc。但是,weak_ptr在进行copy,assing和析构时，只更新the number of weak_ptrs


下面我们来分析一个具体的例子

```cpp
// a.h
#ifndef A_H_
#define A_H_

#include <iostream>
#include <memory>

#include "b.h"

namespace cp {

class A {
 public:
  A() {std::cout << "A() called.\n";}
  ~A() {std::cout << "~A() called.\n";}

  void SetBptr(std::shared_ptr<B> ptr) { b_ptr = ptr;}

 private:
  std::shared_ptr<B> b_ptr;
};

} // namespace cp

#endif // A_H_

// b.h
#ifndef B_H_
#define B_H_

#include <iostream>
#include <memory>

namespace cp {

class A;

class B {
 public:
  B() {std::cout << "B() called.\n";}
  ~B() {std::cout << "~B() called.\n";}

  void SetAptr(std::shared_ptr<A> ptr) {a_ptr = ptr;}

 private:
  std::shared_ptr<A> a_ptr;
};

} // namespace cp

#endif // B_H_

// main.cc
#include "a.h"

using namespace cp;

int main (void) {
  std::shared_ptr<A> a_ptr = std::make_shared<A>();
  std::shared_ptr<B> b_ptr = std::make_shared<B>();

  a_ptr->SetBptr(b_ptr);
  b_ptr->SetAptr(a_ptr);

  return 0;
}

/*
A() called.
B() called.
*/
```

代码逻辑非常简单，典型的shared_ptr cycle reference，导致manage object没有析构。我们具体分析下过程
1. b_ptr先进行析构，(注意，b_ptr是一个smart pointer，不是managed object)，rc--(rc==1，此时还不到析构managed object的时候，即rc!=0)，所以class B没有析构，但是b_ptr已经析构了。
2. a_ptr再进行析构，(注意，由于class B没有析构，所以a_ptr的rc是2)，rc--(rc==1，同上)

下面我们用weak_ptr进行改造
```cpp
// a.h
#ifndef A_H_
#define A_H_

#include <iostream>
#include <memory>

#include "b.h"

namespace cp {

class A {
 public:
  A() {std::cout << "A() called.\n";}
  ~A() {std::cout << "~A() called.\n";}

  void SetBptr(std::shared_ptr<B> ptr) { b_ptr = ptr;}

 private:
  std::shared_ptr<B> b_ptr;
};

} // namespace cp

#endif // A_H_

// b.h
#ifndef B_H_
#define B_H_

#include <iostream>
#include <memory>

namespace cp {

class A;

class B {
 public:
  B() {std::cout << "B() called.\n";}
  ~B() {std::cout << "~B() called.\n";}

  void SetAptr(std::shared_ptr<A> ptr) {a_ptr = ptr;}

 private:
  std::weak_ptr<A> a_ptr;
};

} // namespace cp

#endif // B_H_

// main.cc
#include "a.h"

using namespace cp;

int main (void) {
  std::shared_ptr<A> a_ptr = std::make_shared<A>();
  std::shared_ptr<B> b_ptr = std::make_shared<B>();

  a_ptr->SetBptr(b_ptr);
  b_ptr->SetAptr(a_ptr);

  return 0;
}

/*
A() called.
B() called.
~A() called.
~B() called.
*/
```
下面我们来分析下，上面这段代码和它的输出
1. 为什么class B内部采用weak_ptr而不是class A，这个是有原因的，要根据我们上面总结的第一点进行判断，但是这个demo完全是为了说明问题，所以语义平等，都行。
2. 最后析构的顺序，和一般的stack object first in last out顺序不一致。
3. 先析构b_ptr(此时shared_rc==2, weak_rc == 2)，析构shared_rc--, weak_rc--(shared_rc==1, weak_rc==1)，不对manage object进行析构。
4. 再析构a_ptr(shared_rc==1, weak_rc==2)，shared_rc--(shared_rc==0，weak_rc==1此时会触发对于manage object的析构，即进行class A的析构，调用A的析构函数。所以先输~A() called.由于class A内部包含std::shared_ptr<B>，此时对于该smart pointer再次进行析构，该smart pointer object，shared_rc--，weak_rc--(shared_rc==0, weak_rc==0)触发对于class B的析构，调用B的析构函数，输出~B() called. 并且由于b_ptr的weak_rc==0，所以b_ptr的control block也会发生析构。由于B内部包含weak_ptr<A>，此时weak_rc--,weak_rc==0, ptr_a的control black进行析构)

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

- demo-02

这个case主要用来解决习题。

q:why is `std::initializer_list` often passed by value?
>参考stackoverflow给出的解答，结论就是开销不大，对性能影响不大。

[why is `std::initializer_list` often passed by value?](https://stackoverflow.com/questions/17803475/why-is-stdinitializer-list-often-passed-by-value)

q:为什么常成员函数返回引用时只能返回常引用？非常成员函数返回引用时是否只能返回非常引用?
>对于第一个问题，cpp-primer在7.3给了明确的说明：
A const member function that returns *this as a reference should have a return type that is a reference to const.
>
>进一步来说，const member function会对存入参数this进行修饰，即const this.所以，对于一个常对象，只能通过常引用接受。
>
>对于第二个问题，则不是，非常对象，可以用非常引用也可以用常引用接受。

**注意，上面的理解有不足的地方**看下面的代码

```cpp
// ...
std::string& front();
std::string& front() const;
// ...
```

q: 上面的代码都可以编译通过，并正常运行。这显得非常奇怪在于，为什么一个非常引用可以绑定到常量？
>显然是不行的。
我们都知道const member function本质上修饰this参数，const classtype* this
这里需要注意的是，this并不是常量，常量是*this，即对象本身是常量，那么自然的对象当中的成员也都是常量。
但是，问题在于，如果一个对象内部有指针，那么此时的指针是一个常量，而不是指针指向的成员是常量。
所以，data_是一个std::shared_ptr，他是一个常指针，但是它指向的对象(vector)并不是常量，所以返回这个非常量的首元素，自然不需要一个常引用。

参考<br>
[Returning non-const reference from a const member function](https://stackoverflow.com/questions/5055427/returning-non-const-reference-from-a-const-member-function)

q:In our check function we didn’t check whether i was greater than zero. Why is it okay to omit that check?
>这一道题非常赞，可以思考。
其实合理的范围是[0, size)，但是确实没有判断负数。因为这里采用了无符号数的形式，只要传递进来负数。
大概率会被干掉。但是也有漏记漏洞。-1是最大的负数，那只要这个负数足够小，转换成正数后在[0, size)之间，也是bug.
所以，还是少用无符号数。

q:explicit constructor的优缺点？
>优点，显然，避免automatic conversion,因为后者通常会带来一些hidden bug.
缺点，需要automatic conversion转换的场景，需要显示进行构造，有一点额外的工作量。
但是我觉得额外工作量不重要，因为也没有太多。但是能避免bug则很重要。

[What does the explicit keyword mean?](https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean)

- demo-03

Exercise12.6，我在进行设计时有如下需要单独考虑的问题

1. 指针在进行传递时，没有必要采用google对于未修改传入参数的const T&建议，原因和std::initializer_list一样。直接采用穿值得方式开销不大，it's cheap.
2. 对于std::vector<int>在资源申请时，使用new而不是new []。因为所说语义是数组，但是vector<int>确实是一个class type，采用new 和 delete即可

在对比参考答案后，发现这一小节看书的注意事项没有体现在代码上

1. 如果空间分配不足怎么办？
2. new 和 new std::nothrow的区别是否对比清楚？
3. dangling pointer怎么处理？

上述的讨论会引起另一个问题
q:If make_shared/make_unique can throw bad_alloc, why is it not a common practice to have a try catch block for it?
>参考stackoverflow的答案即可，从答案中也部分说明了，new和make_shared在申请资源失败时碰到的情形一样，
所以，此时不处理，让其异常core,也未尝不可

[If make_shared/make_unique can throw bad_alloc, why is it not a common practice to have a try catch block for it?](https://stackoverflow.com/questions/57158670/if-make-shared-make-unique-can-throw-bad-alloc-why-is-it-not-a-common-practice)

- demo-04

用shared_ptr进行修复

- demo-05

需要关注的几个问题
1. 多个stack object构造和析构的顺序是什么?
2. 多个dynamic object构造和析构的顺序是什么?
3. Derived class在构造和析构时，基类的构造和析构是什么顺序？
4. shared_ptr/ordinary_ptr在资源管理和接口参数的一些细节问题

- demo-06

1. 给出了自己管理socket的raii实现
2. 给出了一个shared_ptr对于socket进行管理

shared_ptr的实现在于没有办法像自己实现的raii类那样，在资源管理对象创建时申请资源，而是得先申请好资源。shared_ptr在进行资源你管理时，要注意
1. shared_ptr内部是plain pointer，所以需要通过地址关联
2. deleter接受的参数T*

- demo-07

在demo-06的基础上，实现了unique_ptr的版本，需要特别注意的是
1. unique_ptr<objT, delT> p (new objT, fcn)
2. 如果callable object是function的形式，需要单独传递一个*表明是function pointer type. decltype只返回function type.

- demo-08

讲了一个shared_ptr导致的cycle reference，从而通过weak_ptr进行解决

- demo-09

这个是书上的例子，考虑到StrBlob作为强引用，提供了一种弱引用的实现。
我猜想作者的这个例子，本质还是为了模仿weak_ptr提供的一些功能，所以，在这个类中，提供的方法也只是对于元素基本的一个的访问.

顺便解决了private函数，通过宏进行测试的方法，比FRIEND_TEST简单一些，代码侵入相对少。