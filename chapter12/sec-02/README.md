[TOC]

## 12.2. Dynamic Arrays

### Basis

q:编译器提供哪些方式让我们在dynamic memory上面一次申请多个对象的空间?
>To support such usage, the language and library provide two ways to allocate an array of objects at once.
1. The language defines a second kind of new expression that allocates and initializes an array of object
2. The library includes a template class named allocator that lets us separate allocation from initialization

q:对于dynamic arrays，作者有怎样的建议？
>Best practices:
Most applications should use a library container rather than dynamically
allocated arrays. Using a container is easier, less likely to contain memorymanagement bugs, and is likely to give better performance.
>
>对于lib container主要由以下一些优点：
1. 资源自动回收
2. 资源动态调整
3. 默认的operations for copy, assignment, destruction

### 12.2.1. new and Arrays

#### Basis

q:new的作用是什么?
>We ask new to allocate an array of objects


We can also allocate an array by using a type alias
```cpp
typedef int arrT[42]; // arrT names the type array of 42 ints
int *p = new arrT; // allocates an array of 42 ints; p points to the first
```

#### Allocating an Array Yields a Pointer to the Element Type

q:rt?
>When we use new to allocate an array, we do not get an object with an array type. Instead, we get a pointer to the element type
of the array.
>
>这里的意思其实想表达，以int元素位例，我们申请了10个整型元素的动态数组，但是我们拿到的并不是一个包好了10个整型元素的动态数组，我们甚至拿到的都不是数组。
我们拿到的是指向元素类型的数组。
>
>这会导致的问题是，我们对于handle的使用，不可能像容器那样，因为它没有迭代的能力。

**Warning**
It is important to remember that what we call a dynamic array does not have
an array type.

#### Initializing an Array of Dynamically Allocated Objects

q:对于dynamic array，我们应该怎样初始化？
1. By default, objects allocated by new—whether allocated as a single object or in an array—are default initialized.
2. We can use value initializer.
3. Under the new standard, we can also provide a braced list of element initializers

```cpp
int *pia = new int[10]; // block of ten uninitialized ints
int *pia2 = new int[10](); // block of ten ints value initialized to 0
string *psa = new string[10]; // block of ten empty strings
string *psa2 = new string[10](); // block of ten empty strings
int *pia3 = new int[10]{0,1,2,3,4,5,6,7,8,9};
```

q:我们可以使用value initialize，我们是否可以使用element initializer?
>不行。
我们先来回顾dynamic object，
When we provide an initializer inside parentheses, we can use auto (§ 2.5.2, p. 68)
to deduce the type of the object we want to allocate from that initializer.

```cpp
int* pval = new int(3); // *pval = 3
auto pval1 = new auto(3); // *pval1 = 3
```

>对于dynamic array
Although we can use empty parentheses to value initialize the elements of an array,
we cannot supply an element initializer inside the parentheses, which means that we cannot use auto
to allocate an array

#### It Is Legal to Dynamically Allocate an Empty Array

这里是我觉得非常trick的地方

```cpp
char arr[0]; // error: cannot define a zero-length array
char *cp = new char[0]; // ok: but cp can't be dereferenced
```

1. When we use new to allocate an array of size zero, new returns a valid, non-zero pointer.
2. That pointer is guaranteed to be distinct from any other pointer returned by new.
3. This pointer acts as the off-the-end pointer (§ 3.5.3, p. 119) for a zero-element array
4. We can use this pointer in ways that we use an off-the-end iterator. The pointer can be compared as in the loop above.

结合以上四点，我觉得这么申请来的pointer，比较类似迭代器当中end的概念，它其实也不指向任何元素，但是它本身也是由意义的，就是作为容器结尾的下一个元素(但是这里实际没有元素，类似哨兵的概念)

#### Freeing Dynamic Arrays

q:rt?
>To free a dynamic array, we use a special form of delete that includes an empty pair of square brackets.
If we omit the brackets when we delete a pointer to an array (or provide
them when we delete a pointer to an object), the behavior is undefined

**Warning**
The compiler is unlikely to warn us if we forget the brackets when we
delete a pointer to an array or if we use them when we delete a pointer
to an object. Instead, our program is apt to misbehave without warning
during execution.

#### Smart Pointers and Dynamic Arrays

一些点注意下就好：
1. The library provides a version of unique_ptr(使用new [], delete []，而不是new and delete) that can manage arrays allocated by new.(不用shared_ptr的原因在于，dynamic array一般没有copy, assignment语义，当然，不是特殊情况也可以用)
2. unqiue_ptrs that point to arrays provide slightly different operations than those(dynamic array 和 dynamic object有不同的操作，这很自然，数组需要遍历对象又不需要)
3. Unlike unique_ptr, shared_ptrs provide no direct support for managing a dynamic array. If we want to use a shared_ptr to manage a dynamic array, we must provide our own deleter.

### 12.2.2. The allocator class

#### Basis

q:使用new进行dynamic object/dynamic array进行资源申请，有什么缺点?
>new combines allocating memory with constructing object(s) in that memory.
Similarly, delete combines destruction with deallocation.

q:new的这种方式，适用场景是什么?
>申请内存时，进行初始化，可以省掉自己再次进行初始化，虽然不灵活，但是更方便。所以，优点也是缺点。具体问题，要看适用的场景。
Combining initialization with allocation is usually what we want when we allocate a single object. In that case, we almost certainly know the value the object should have

q:new的这种方式，不使用场景是什么?
>
1. When we allocate a block of memory, we often plan to construct objects in that memory as needed.
2. 一个常见的场景是，我们申请了一个整型数组(采用new)，但是我们在使用时，需要在逐一进行赋值，所以此时没有必要在开辟空间时就进行初始化。为什么不能在使用new时，就直接初始化为需要的值呢？因为new在开辟dynamic array时，只支持value initializer，不支持element initializer.所以，如果使用new，相当于每个元素初始化了2遍，但是allocate只用一遍。
3. 还有，如果一个class type，没有提供default constructor，那么它不能使用new直接分配

所以，对于dynamic array, coupling allocation and construction can be wasteful

#### The allocator Class

q:allocator的用法是什么?
1. The library allocator class, which is defined in the memory header, lets us separate allocation from construction.
2. It provides type-aware allocation of raw, unconstructed, memory.
3. Like vector, allocator is a template. To define an allocator we must specify the type of objects that a particular allocator can allocate

#### allocators Allocate Unconstructed Memory

q:allocator如何对分配的内存进行初始化?
>In the new library the construct member takes a pointer and zero or more additional arguments
it constructs an element at the given location
>
>特别注意这个given location，很显然，allocator提供了非常细粒度的初始化能力，对于一块dynamic array，可以针对具体的某一个位置的对象，进行初始化

```cpp
auto q = p; // q will point to one past the last constructed element
alloc.construct(q++); // *q is the empty string
alloc.construct(q++, 10, 'c'); // *q is cccccccccc, 这种是新版本construct
alloc.construct(q++, "hi"); // *q is hi!
```

**Warning**
We must construct objects in order to use memory returned by allocate. Using unconstructed memory in other ways is undefined.

q:allocator如何对分配的内存进行析构?
>The destroy function takes a pointer and runs the destructor (§ 12.1.1, p. 452) on the
pointed-to object

```cpp
while (q != p)
  alloc.destroy(--q);
```

**Warning**
We may destroy only elements that are actually constructed.

q:allocator如何对于内存进行回收?
>We free the memory by calling deallocate

```cpp
alloc.deallocate(p, n);
```

#### Algorithms to Copy and Fill Uninitialized Memory

q:对于raw memory，library还提供了哪些方式进行构造？
>As a companion to the allocator class, the library also defines two algorithms that can construct objects in uninitialized memory.
