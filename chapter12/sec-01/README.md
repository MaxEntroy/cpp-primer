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

#### A shot summary

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