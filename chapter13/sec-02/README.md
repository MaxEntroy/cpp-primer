[TOC]

## Section13.2 Copy Control and Resource Management

### Basis

q:一般什么时候需要定义copy control members?
1. Ordinarily, classes that manage resources that do not reside in the class must define
the copy-control members.
2. 1中是最常见的情形，还有一些需要看特殊的copy semantics，比如我们上一小节遇到的需要copy/assign增加对象unique id的case.

q:当我们定义copy control members时，首先需要考虑的是什么?
>copy semantics:
1. (valuelike semantic)We can define the copy
operations to make the class behave like a value
2. (pointerlike semantic)We can define the copy
operations to make the class behave like a pointer

q:valuelike语义，对应的具体操作是什么?
>Classes that behave like values have their own state. When we copy a valuelike
object, the copy and the original are independent of each other. Changes made to the
copy have no effect on the original, and vice versa.
>
>一个显而易见的例子是std::string，它管理dynamic memory，但是copy semantics实现的是valuelike

q:pointerlike语义，对应的具体操作是什么?
>Classes that act like pointers share state. When we copy objects of such classes, the
copy and the original use the same underlying data. Changes made to the copy also
change the original, and vice versa.
>
>一个显而易见的例子是std::shared_ptr<T>，它管理dynamic memeory，但是copy semantics实现的是pointerlike

q:对于一个built-in type，该如何让他具有pointerlike semantics?
>这是一个错误的问题，build-in type是value，它的copy semantics就是value like，当然，具体原因是因为built-in type开销小(注意，std::string, std::vector...这些可不是built-in type，因为他们需要额外的头文件)
1. Ordinarily,
classes copy members of built-in type (other than pointers) directly; such members
are values and hence ordinarily ought to behave like values
2. What we do when we
copy the pointer member determines whether a class like HasPtr has valuelike or
pointerlike behavior.

### 13.2.1. Classes That Act Like Values

q:valuelike semantic如何管理资源？
>To provide valuelike behavior, each object has to have its own copy of the resource
that the class manages.

#### Valuelike Copy-Assignment Operator

q:assignment operators有什么需要特别注意的地方？
**Assignment operators typically combine the actions of the destructor and the copy
constructor.**
1. Like the destructor, assignment destroys the left-hand operand’s
resources.
2. Like the copy constructor, assignment copies data from the right-hand
operand

**Key Concept: Assignment Operators**
There are two points to keep in mind when you write an assignment
operator:
- Assignment operators must work correctly if an object is assigned to itself.
- Most assignment operators share work with the destructor and copy
constructor.

**A good pattern** to use when you write an assignment operator is to first copy
the right-hand operand into a local temporary. After the copy is done, it is
safe to destroy the existing members of the left-hand operand. Once the lefthand
operand is destroyed, copy the data from the temporary into the
members of the left-hand operand.

下面我们看一段错误代码：
```cpp
// WRONG way to write an assignment operator!
HasPtr&
HasPtr::operator=(const HasPtr &rhs)
{
delete ps; // frees the string to which this object points
// if rhs and *this are the same object, we're copying from deleted memory!
ps = new string(*(rhs.ps));
i = rhs.i;
return *this;
}
```

### 13.2.2. Defining Classes That Act Like Pointers

q:pointerlike semantic面临的问题是什么？
>对于pointerlike semantic，虽然copy的步骤变得简单，但是析构的流程变得复杂：
1. the destructor cannot unilaterally free its associated string. It can do so only when the
last HasPtr pointing to that string goes away.

不能直接进行析构的原因在于，需要确定是否还有其他对象在共享动态开辟的资源，否则某一个对象析构会导致其余对象不可用。

q:上述问题怎么解决?
1. The easiest way to make a class act like a pointer is to use shared_ptrs to
manage the resources in the class.
2. However, sometimes we want to manage a resource directly. In such cases, it can
be useful to use a reference count

#### Reference Counts

Reference counting works as follows:
- In addition to initializing the object, each constructor (other than the copy
constructor) creates a counter. This counter will keep track of how many objects
share state with the object we are creating. When we create an object, there is
only one such object, so we initialize the counter to 1.
- The copy constructor does not allocate a new counter; instead, it copies the
data members of its given object, including the counter. The copy constructor
increments this shared counter, indicating that there is another user of that
object’s state.
- The destructor decrements the counter, indicating that there is one less user of
the shared state. If the count goes to zero, the destructor deletes that state.
- The copy-assignment operator increments the right-hand operand’s counter and
decrements the counter of the left-hand operand. If the counter for the left-hand
operand goes to zero, there are no more users. In this case, the copyassignment
operator must destroy the state of the left-hand operand.

#### Defining a Reference-Counted Class

q:The only wrinkle is deciding where to put the reference count?
1. 如果存放在stack memory，不同对象无法通信
2. 根据12.1.1当中的内容，我们选择在dynamic memory当中存放reference count

Programs tend to use dynamic memory for one of three purposes:
1. They don’t know how many objects they’ll need
2. They don’t know the precise type of the objects they need
3. They want to share data between several objects

#### Pointerlike Copy Members “Fiddle” the Reference Count

q:这里提到fiddle的作用是什么?
>增加引用计数，并不是真的增加dynamic memory object的数量，而是指向这块内存的数量。

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

- demo-05

课后习题实现，strblob的valuelike version.

实现的时候，为了说明本节问题，只写了copy control members。对于operator=如何析构left operand，参考cpp-reference:
std::shared_ptr is a smart pointer that retains shared ownership of an object through a pointer. Several shared_ptr objects may own the same object. The object is destroyed and its memory deallocated when either of the following happens:
1. the last remaining shared_ptr owning the object is destroyed;
2. the last remaining shared_ptr owning the object is assigned another pointer via operator= or reset().

根据2中所讲，直接进行赋值即可析构

```cpp
StrBlob& StrBlob::operator=(const StrBlob& rhs) {
  if (this == &rhs) {
    return *this;
  }

  auto sp = std::make_shared<VecStr>(*rhs.data_);
  if (!sp) {
    return *this;
  }

  data_ = sp;
  return *this;
}
```

- demo-06

对于同样的问题，我实现了自己的版本

- demo-07

又实现了一版本，因为对于题目，我自己并没有领会题目的意思。
1. 本质还是想考察对于copy semantics的理解，valuelike or pointerlike，具体来说只对于object当中的pointer member才需要考虑，其余的则不需要。
2. 看了答案的实现，发现它的思路在于，构造的时候用户负责，但是析构的时候，除了root由programmer显示析构，其余部分则是由程序自动析构。这么做会显得new/delete不对称，但是也是不错的实现
3. 这个题目本质是，如何拷贝一个binary tree(pointerlike version)

下面说下我的实现，我实现了一个错误的版本，问题出在，一开始就没有从全局着眼去把问题想清楚。导致到最后才发现，非常的不好。但是，也有些收货
1. valuelike or pointerlike本质的区别是针对Pointer成员。所以，我一开始实现的时候BiNode的时候，没想明白。为什么pleft和pright的ref_cnt需要增加，但是本身的成员却不增加。其实，自己的成员不放换个名字，从逻辑上梳理更清楚。总之，这二者的区别针对的是pointer成员，ref和val开辟在dynamic memory上
2. 我看了acc copy control的实现，发现它的实现先析构，再赋值。当然，他判断了自赋值。书上这么做是为了避免自赋值，但是我觉得它这么做确实有优点(新内存开辟不成功，不再进行赋值)但是，这个例子实现的是pointerlike，所以不影响，我采用了先析构再赋值的顺序，没有教条。

对于这个问题我的解决，我觉得本质需要搞懂bitree的copy semantics(基于pointerlike)，我了解，其实对于bitree的赋值和复制，就是对于proot成员进行pointerlike实现即可。其余binode没有赋值和复制的意义，因为逻辑上整个bitree就是一个根节点的指针表示。其余根节点的指针代表lchild,rchild，所以内在还是有一个区别。

看了书上的答案，和网上的答案，我觉得语义都不清楚，按照我自己的思路来就好。