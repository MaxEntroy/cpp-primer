[TOC]

## Section13.6. Moving Objects

### Basis

q:why moving objects?
1. When an object is immediately destroyed after it is copied, moving, rather than copying, the object can provide a significant
performance boost.
2. When classes(unique_ptr or IO buffer) have a resource that may not be shared, objects of these types
can’t be copied but can be moved.

### 13.6.1. Rvalue Rerefences

q:motivation?
>To support move operations
1. Move semantics作用于即将被析构，但是要拷贝的对象
2. 新标准引入了rvalue ref来表示即将被析构，但是要拷贝的对象。即，对于一个右值引用，我们可以放心的使用move operation.

q:what is rvalue ref?
>An rvalue reference is a reference that must be bound to an
rvalue.

q:rvalue ref declaration?
>An rvalue reference is obtained by using && rather than &

q:property of rvalue ref?
>rvalue references have the important property that they may be bound only to an object that
is about to be destroyed.As a result, we are free to “move” resources from an rvalue
reference to another object.

#### Lvalues Persist; Rvalues Are Ephemeral

q:diff?
1. Lvalues have persistent state
2. rvalues are either literals or temporary objects

q:对于rvalue ref关联的对象，有什么特点？
1. The referred-to object is about to be destroyed
2. There can be no other users of that object

####　Variables Are Lvalues

ｑ:rt(讨论这个话题意义何在)?
>Although we rarely think about it this way, a variable is an expression with one
operand and no operator.
>
>这恰恰也从侧面印证了，其实每一条语句都还是一条expression.所有的statement本质上都是在做计算

```cpp
int a = 3; // 右值做计算，只不过只有一个操作数且没有操作符
print(3, 4); // 我们丢弃了左值，但本质也是有左值的
```

**we cannot bind an rvalue reference to a variable defined as an rvalue**

```cpp
int &&rr1 = 42; // ok: literals are rvalues
int &&rr2 = rr1; // error: the expression rr1 is an lvalue!
```

#### The Library move Function

q:motivation?
>Although we cannot directly bind an rvalue reference to an lvalue, we can explicitly
cast an lvalue to its corresponding rvalue reference type.
>
>我们需要想办法把右值引用绑定到一个左值上面，至于为什么要这么做，这个我们稍后讨论

q:std::move()的作用是什么?
>The move function uses facilities that we’ll describe in
§ 16.2.6 (p. 690) to return an rvalue reference to its given object.
>
>所以，对于一个左值，我们通过调用std::move(lvalue)可以得到一个rvalue ref，从而把rvalue ref绑定到了一个lvalue上

```cpp
int &&rr1 = 42; // ok: literals are rvalues
int &&rr2 = rr1; // error: the expression rr1 is an lvalue!
int &&rr3 = std::move(rr1); // ok
```
q:std::move further discussion?
1. Calling move tells the compiler that we have an lvalue that we want to treat as if it
were an rvalue.
2. It is essential to realize that the call to move promises that we do not
intend to use rr1 again except to assign to it or to destroy it.
3. After a call to move, we
cannot make any assumptions about the value of the moved-from object.

简单总结下：
1. std::move的作用是获取一个左值的右值引用，这样就可以把一个右值引用绑定到左值上
2. 为什么要把一个右值引用绑定到左值上，原因在于存在一些即将析构，但是要拷贝的左值(虽然他们不是temporary object，但是语义上和temporary object是一样的)，对于这一类左值，我们也希望可以move他们的资源
3. 对于被移动资源的左值，必须确保该对象出在valid but destructible的状态。
3.1. We can destroy a moved-from object and can assign a new value to it(valid)
3.2. we cannot use the value of a moved-from object.(but destructible)

q:why std::move not move
>Code that uses move should use std::move, not move. Doing so avoids
potential name collisions.

### 13.6.2. Move Constructor and Move Assignment

q:move constructor/move assignment有什么需要注意的地方？
1. 参数是右值引用
2. 需要保证moved-from object in a valid but destructible state.

```cpp
StrVec::StrVec(StrVec &&s) noexcept // move won't throw any exceptions
// member initializers take over the resources in s
: elements(s.elements), first_free(s.first_free),
  cap(s.cap)
{
  // leave s in a state in which it is safe to run the destructor
  s.elements = s.first_free = s.cap = nullptr;
}
```

#### Move Operations, Library Containers, and Exceptions

这一小节来解释下，为什么move constructor需要用noexcept来声明

q:为什么move constructor需要声明noexcept?
>Because a move operation executes by “stealing” resources, it ordinarily does not itself
allocate any resources. As a result, move operations ordinarily will not throw any
exceptions.
>
>既然操作上不会抛异常，那么声明清楚，对于使用者和调用者语义上更清晰。当然，这里其实更多的还是从自身的角度来说。

q:反正move operations也不会抛异常，那么是否声明又有什么影响呢？
>unless the library knows that our move constructor
won’t throw, it will do extra work to cater to the possibliity that moving an object of
our class type might throw.
>
>如果move constructor不声明noexcept，那么对于调用者来说(library)，它不会去看你的代码判断你到底会不会抛异常，
只以move constructor是否声明为准，如果不声明，那么library会认为move constructor会抛出异常。
这样library会做一些额外的工作。如果，声明清楚，那么这部分工作library可以节省。
>
>因为move constructor也确实不抛异常，那么就没有必要让library再做这部分工作了

**Move constructors and move assignment operators that cannot throw
exceptions should be marked as noexcept.**

q:书上讲了vector.push_back这个例子的用意何在
>用意是，move constructor不声明noexcept是不行的，因为如果不声明，那么一旦这个library是vector，并且调用push_back导致realloction的时候，vector是不会调用
我们自定义类型的move operation，只能调用copy operation.

1. move operation虽然不跑出异常，但是它被允许可以抛出异常
2. library在user defined type的操作抛出异常时，它需要定义额外的操作。
2.1. 以vector为例，它要求一旦user defined type抛出异常后，此时容器应该要保证之前的状态不受到影响
2.2. 如果move operation没有声明noexcept，但是它抛出了异常，它会导致容器之前的状态遭到破坏，从而vector的要求被破坏
2.3. 如果copy operation没有声明noexcept，但是它跑出了异常，它不会导致容器之前的状态遭到破坏，从而不破坏vector的要求
2.4. 按照书上的意思，vector应该是有这个要求，但是它实际没有做额外的工作，所以他会判断，到底哪些操作会破坏它的要求，因为vector实现可能不做额外工作，那么它就要保证这一类的操作不能被调用
2.5. 从而，move operation不会被vector调用，除非它声明为noexcept，即vector调用它可以保证自己的要求不被破坏

#### Move-Assignment Operator

```cpp
StrVec &StrVec::operator=(StrVec &&rhs) noexcept
{
  // direct test for self-assignment
  if (this != &rhs) {
  free(); // free existing elements
  elements = rhs.elements; // take over resources from rhs
  first_free = rhs.first_free;
  cap = rhs.cap;
  // leave rhs in a destructible state
  rhs.elements = rhs.first_free = rhs.cap = nullptr;
  }
  return *this;
}
```

q:上面这段代码有什么疑惑的地方？
1. rhs是一个右值引用，虽然rhs是一个变量，可以取地址，但是右值不能取地址，所以rhs也不可以取地址才对
2. 如果是自赋值，那么语义上会矛盾，等号右边的是一个右值(等号左边也可能是左值)，右值给右值自赋值，那就表示这个右值要出现在左值的位置上，这回造成语义矛盾

a:
1. 对于第一个问题，由于rhs可能是std::move的结果，那么可能是一个lvalue
2. 如果是一个temporary object，肯定不会有自赋值的场景，因为temporary object没有entity，所以还是一个lvalue。话句话说，既然能调用这个函数，那么此时的左值一定是存在的

#### A Moved-from Object Must Be Destructible

q:rt?

**After a move operation, the “moved-from” object must remain a valid,
destructible object but users may make no assumptions about its value.**

#### The Synthesized Move Operations

这里我们先回顾一下copy constructor/copy assignment operator/destructor的一些细节：
1. 如果用户自定义copy constructor/copy assignment operator/destructor，那么编译器不会为它合成
2. 如果用户不定义copy constructor/copy assignment operator/destructor，那么编译器一定会为它合成。但是这里要注意的是：
2.1. Implicitly-declared copy constructor/copy assignment operator/destructor(If the implicitly-declared copy constructor is not deleted, it is defined )
2.2. Deleted implicitly-declared copy constructor/copy assignment operator/destructor

需要我们注意的是：
1. 编译器在合成的时候，是有可能声明为delete。这代表着，当我们依赖编译器为我们合成的时候，编译器有可能合成一个deleted function，从而不能进行copy
2. 以上三个copy control member在编译器自动合成时，只依赖自身是否有用户显示定义，而不是整个copy control member

q:哪些场景编译器会自动合成move constructor/move assignment operator?
1. The compiler will synthesize a move constructor or a move-assignment operator
only if the class doesn’t define any of its own copy-control members
2. and if every nonstatic data member of the class can be moved

对比以上3个copy control member，这里不仅仅是不能定义自身，并且copy control member当中的任意一被显示定义，都不能合成move operations。不自动合成的不再赘述

q:当编译器自动合成move operations时，什么场景会声明为deleted?
>Unlike the copy operations, a move operation is never implicitly defined as a deleted
function.
>
>划重点，这里特别注意，行为和前3个copy control member又不一样。
1. Unlike the copy constructor, the move constructor is defined as deleted if the
class has a member that defines its own copy constructor but does not also
define a move constructor, or if the class has a member that doesn’t define its
own copy operations and for which the compiler is unable to synthesize a move
constructor. Similarly for move-assignment.
2. The move constructor or move-assignment operator is defined as deleted if the
class has a member whose own move constructor or move-assignment operator
is deleted or inaccessible.
3. Like the copy constructor, the move constructor is defined as deleted if the
destructor is deleted or inaccessible.
4. Like the copy-assignment operator, the move-assignment operator is defined as
deleted if the class has a const or reference member.

这里面3和4好理解，对于1和2，要特别小心的是，这里说的都是成员，而不是从copy control member的角度去说的。

编译器的自动合成行为，以下几个链接讲的比较清楚<br>
[destructor](https://en.cppreference.com/w/cpp/language/destructor)<br>
[copy_constructor](https://en.cppreference.com/w/cpp/language/copy_constructor)<br>
[copy_assignment](https://en.cppreference.com/w/cpp/language/copy_assignment)<br>
[move_constructor](https://en.cppreference.com/w/cpp/language/move_constructor)<br>
[move_assignment](https://en.cppreference.com/w/cpp/language/move_assignment)<br>

#### Rvalues Are Moved, Lvalues Are Copied ...

q:rt?
>When a class has both a move constructor and a copy constructor, the compiler uses
ordinary function matching to determine which constructor to use

```cpp
StrVec v1, v2;
v1 = v2; // v2 is an lvalue; copy assignment
StrVec getVec(istream &); // getVec returns an rvalue
v2 = getVec(cin); // getVec(cin) is an rvalue; move assignment
```

#### ...But Rvalues Are Copied If There Is No Move Constructor

q:但是copy constructor的接口和move constructor的接口不同，又为什么能调用呢？
>参考demo-02，copy construtor/copy assignment的参数是const T&，可以绑定到rvalue上。所以接口是兼容的。
这也是为什么copy semantics的两个接口都必须采用const T&的原因

#### Copy-and-Swap Assignment Operators and Move

q:对于HasPtr，我们应该如何支持move operations?
这里需要特别强调的是，HasPtr的operator=实现采用了swap technique，这也导致对于move assignment operator的实现有另外一种风格

我们先讨论正常的版本，copy operator=不采用swap technique
```cpp
~HasPtr() {
  if(pstr_) {
    delete pstr_;
  }
}

HasPtr(const HasPtr&);
HasPtr& operator=(const HasPtr&);
HasPtr(HasPtr&&);
HasPtr& operator=(HasPtr&&);

HasPtr v1, v2;
v1 = v2; // 参数是lvalue，只能调用copy assignment

v1 = GetHasPtr() // 参数是rvalue，有move assignment则优先调用
```

看swap technique的版本
```cpp
~HasPtr() {
  if(pstr_) {
    delete pstr_;
  }
}

HasPtr(const HasPtr&);
HasPtr& operator=(HasPtr); // 参数不是引用，不会直接匹配左值引用或右值引用。值传递兼顾了copy assighment and move assignment
HasPtr(HasPtr&&);

HasPtr v1, v2;
v1 = v2; // v2是实参，左值，调用copy constructor初始化形参。形参要发生valuelike copy

v1 = std::move(v2) // v2是左值，不是实参。std::move(v2)是右值，是实参，调用move constructor初始化形参。形参移动std::move(v2)资源，没有发生资源重新分配
// 能这么用的前提一定要清楚，v2之后就不能用了
```

**Updating the Rule of Three**

>All five copy-control members should be thought of as a unit: Ordinarily, if a
class defines any of these operations, it usually should define them all.

上面这个其实跟具体讲的也不完全一样，我总结一个我认可的版本

- Classes That Need Destructors Need Copy and Assignment
- Classes That Need Copy Need Assignment, and Vice Versa
- Classes that define the move constructor and move-assignment operator can avoid this overhead in
those circumstances where a copy isn’t necessary.(即当我们有必要定义copy operation的时候，也定义move operation)

#### Move Operations for the Message Class

q:本小节的意义?
>Classes that define their own copy constructor and copy-assignment operator
generally also benefit by defining the move operations.

#### Move Iterators

q:rt?
>对于iterator，采用deference会返回lvalue，参考我们realloc_mem的代码
这导致我们不能使用uninitialized_copy

```cpp
void StrVec::realloc_mem() {
  int cap = capacity();
  int new_cap = cap * 2;
  Iter new_first = alloc_.allocate(new_cap);
  Iter new_last = new_first;

  // std::uninitialized_copy(first_, last_, new_last_);
  for(Iter it = first_; it != last_; ++it) {
    alloc_.construct(new_last++, std::move(*it));
  }

  uncreate();

  first_ = new_first;
  last_ = new_last;
  end_ = first_ + new_cap;
}
```

q:make_move_iterator的作用是什么？
>This function takes an iterator and returns a move
iterator
>
>the dereference operator of a move iterator yields
an rvalue reference.
>
>使用它的语义是一样的，如果要用move iterator，必要保证iterator所关联的对象，之后不再使用

q:有哪些坑？
- It is worth noting that standard library makes no guarantees about which algorithms
can be used with move iterators and which cannot.
- Because moving an object can
obliterate the source, you should pass move iterators to algorithms only when you are
confident that the algorithm does not access an element after it has assigned to that
element or passed that element to a user-defined function.

**Advice: Don’t Be Too Quick to Move**

1. Because a moved-from object has indeterminate state, calling std::move on
an object is a dangerous operation. When we call move, we must be
absolutely certain that there can be no other users of the moved-from object.
2. Judiciously used inside class code, move can offer significant performance
benefits. Casually used in ordinary user code (as opposed to class
implementation code), moving an object is more likely to lead to mysterious
and hard-to-find bugs than to any improvement in the performance of the
application

**Best Practices**

Outside of class implementation code such as move constructors or
move-assignment operators, use std::move only when you are certain
that you need to do a move and that the move is guaranteed to be safe.

### 13.6.3. Rvalue References and Member Functions

### 实践

- demo-01

在sec-05-demo-1基础上，修改了如下问题：
1. 修复了create(ConstIter b, ConstIter e)
2. 支持move operation
3. realloc_mem使用std::move(std::string);

q:碰到一个问题，返现move constructor无法触发？

```cpp
StrVec GetStrVec() {
  return v;
}
StrVec v = GetStrVec();
// 这一段代码无法触发move constructor
```

1. 具体原因参考[Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)
2. operator assignment没有问题
3. GetStrVec返回一个左值，这没问题。但是从表达式来看，GetStrVec的调用显然会生成一个临时对象，毋庸置疑

q:那么如何调用move constructor呢？
```cpp
StrVec GetStrVec() {
  return std::move(v);
}
// 1. std::move作用于即将被析构的lvalue
// 2. v符合这个条件，从而临时对象直接移动v的资源
// 3.最后的结果少了一次构造，不管是否使用std::move，所以这里我们不清楚编译器做了何种优化
```

**std::move可以作用于lvalue来产生rvalue ref.**

参考<br>
[is-an-object-guaranteed-to-be-moved-when-it-is-returned](https://stackoverflow.com/questions/11088023/is-an-object-guaranteed-to-be-moved-when-it-is-returned)
[c11-rvalues-and-move-semantics-confusion-return-statement](https://stackoverflow.com/questions/4986673/c11-rvalues-and-move-semantics-confusion-return-statement?lq=1)

- demo-02

主要对于Rvalues Are Copied If There Is No Move Constructor来做实验

```cpp
cp::StrVec v1({"hello", "world", "hello", "cpp"});
v = v1;
EXPECT_EQ(v.size(), 4);
EXPECT_EQ(v.capacity(), 8);

cp::StrVec v2;
v2 = GetStrVec();
/*
StrVec& operator= called.
GetStrVec() called.
StrVec(StrVec&) called. // v产生临时对象，v是lvalue
StrVec& operator=(StrVec&&) called. // 临时对象赋值给v2，调用move assignment
*/
```

按照实验要求，我们禁止move assignment
```cpp
/*
StrVec& operator= called.
GetStrVec() called.
StrVec(StrVec&) called.
StrVec& operator= called. // copy assignment被调用
*/
```

但是，如果我们修改copy assignment的声明
```cpp
StrVec& operator=(StrVec&);
/*
no match for 'operator=' (operand types are 'cp::StrVec' and 'cp::StrVec')
*/
```
原因在于，non-const lvalue ref不能绑定到rvalue，所以，对于copy control member的参数，采用const ref是有原因的。

- demo-03

实现move iterator