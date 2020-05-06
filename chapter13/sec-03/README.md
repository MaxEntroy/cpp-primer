[TOC]

## Section13.3. Swap

### Basis

q:swap的作用是什么?
>Defining swap is particularly important for classes that we plan to use with algorithms that reorder elements
>
>排序的时候，因为会交换两个元素，所以会用到。

q:重新定义swap的时机是什么？
>In addition to defining the copy-control members, classes that manage resources often
also define a function named swap
>
>因为swap操作牵扯到copy and assign，所以如果一个类要自己管理资源，那么重定义swap操作可以优化代码性能(默认的swap是valuelike，但是管理资源的类，通常要通过Low-level pointer实现，此时swap通过valuelike开销较大，可以直接交换管理资源的指针，来避免管理对象的拷贝)

q:swap的lib版本和自己定义的版本有什么关系?
>If a class defines its own swap, then the algorithm uses that class-specific version.
Otherwise, it uses the swap function defined by the library.

q:对于管理资源的类，如何定义swap函数，基本原则是什么?
>In principle, none of this memory allocation is necessary.we’d like swap to swap the pointers
>
>这里还需要注意一点的事，只有实现valuelike version，swap的定义才是合理的。

**Note:**
Unlike the copy-control members, swap is never necessary. However,
defining swap can be an important optimization for classes that allocate
resources.

#### Writing Our Own swap Function

q:rt?
1. We start by declaring swap as a friend
2. Because swap exists to optimize our code, we’ve defined swap as an inline function

```cpp
class HasPtr {
friend void swap(HasPtr&, HasPtr&);
// other members as in § 13.2.1 (p. 511)
};

inline
void swap(HasPtr &lhs, HasPtr &rhs)
{
  using std::swap;
  swap(lhs.ps, rhs.ps); // swap the pointers, not the string data
  swap(lhs.i, rhs.i);   // swap the int members
}
```

#### swap Functions Should Call swap, Not std::swap

**There is one important subtlety in this code: Although it doesn’t matter in this
particular case, it is essential that swap functions call swap and not std::swap.**

以上结论暂且记住。

1. 对于上面的代码，使用std::swap到没有什么问题，因为交换的都是built-in type.
2. 但是，如果class当中有用户自定义类型，且定义了swap函数，那么会出现错误，即不会调用用户定义的版本(使用默认版本会带来额外的开销)

**Each call to swap must be unqualified**

#### Using swap in Assignment Operators

q:什么是copy and swap technique?
>This technique swaps the left-hand
operand with a copy of the right-hand operand

```cpp
// note rhs is passed by value, which means the HasPtr copy constructor
// copies the string in the right-hand operand into rhs
HasPtr& HasPtr::operator=(HasPtr rhs)
{
  // swap the contents of the left-hand operand with the local variable rhs
  swap(*this, rhs); // rhs now points to the memory this object had used
  return *this; // rhs is destroyed, which deletes the pointer in rhs
}
```

- pros
  - swap操作对于实现valuelike copy，没有额外开销(正常的流程是，根据右值进行对象开辟，析构左值，然后右值指针赋给左值)
  - it automatically handles self assignment and is automatically exception safe.
- cons
  - 右值会进行拷贝操作，如果valuelike copy会引起新的资源分配
  - 需要类提供copy constructor

我简单总结下：
- when
  - 资源管理类
  - valueLike copy semantics
- how
  - declaration: friend void swap(T&, T&);(注意是override，所以和std::swap标签要一致)
  - definition: inline void swap() {}
  - Each call to swap must be unqualified
- ps
  - assignment operator can be implemented in copy and swap technique.(pointerlike copy semantics will not benefit from defining a swap function)

### 实践

- demo-01

实现了自己的swap版本，并且assignment operator采用copy and swap实现.

对于inline的使用，需要特别注意当我们声明一个non-member function to be inline的时候
1. 声明时不加inline keyword
2. 定义时增加inline keyword
3. 如果inline function要给别的.cc文件使用，必须定义在header file当中

参考
[Inline Functions](https://isocpp.org/wiki/faq/inline-functions)

- demo-02

课后习题，定义operator< 来判断swap的时机，这道题非常好。有以下几个问题需要考虑：
1. vector.push_back，调用default initialization/copy initialization?
2. std::sort调用swap，为什么不直接调用std::swap，而是调用了HasPtr::swap?

第一个问题，我补充在sec-01当中了。对于第二个问题，我们注意到我们对swap的重新定义，本质是override。所以，如果有重写版本，会先调用重写版本