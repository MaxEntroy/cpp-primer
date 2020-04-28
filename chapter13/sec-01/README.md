## 13.1. Copy, Assign, and Destroy

### 13.1.1. The Copy Constructor

#### Basic

1. A constructor is the copy constructor if its first parameter is a reference to the class type and any additional parameters have default value
2. That parameter is almost always a reference to const
3. the copy constructor usually should not be explicit

#### The Synthesized Copy Constructor

q:synthesiszed copy constructor和synthesiszed default constructor有什么区别?
>Unlike the synthesized default constructor, a copy constructor is synthesized even if we define other constructors.
>
1. synthesized default constructor不会被编译器合成，如果定义了别的构造函数
2. synthesized copy constructor会被编译器合成，即使定义了别的构造函数

```cpp
class TextQuery {
 public:
  explicit TextQuery(const std::string& path);
  ~TextQuery();

  TextQuery(const TextQuery&) = delete;
  TextQuery& operator=(const TextQuery&) = delete;

  TextQuery(TextQuery&&) = default;
  TextQuery& operator=(TextQuery&&) = default;

 public:
  bool Init();

  void EventLoop();

 private:
  std::string file_path_;
  std::unique_ptr<TextQueryImpl> pimpl_;
};

int main() {
  TextQuery tq; // wrong, no defalut constructor is synthesized.
  return 0;
}
```

q:编译器在生成synthesized copy constructor的默认行为是什么?
1. the synthesized copy constructor for some classes prevents us from copying objects of that class type.
2. Otherwise, the synthesized copy constructor memberwise copies the members of its argument into the object being created

q:synthesized copy constructor是否会处理static member?
>No.The compiler copies each non static member in turn from the given object into the one being created.

#### Copy Initialization

q:direct initialization vs copy initialization?
1. When we use direct initialization, we are asking the compiler to use ordinary function
matching (§ 6.4, p. 233) to select the constructor that best matches the arguments
we provide.
2. When we use copy initialization, we are asking the compiler to copy
the right-hand operand into the object being created, converting that operand if
necessary

```cpp
string dots(10, '.'); // direct initialization
string s(dots); // direct initialization
string s2 = dots; // copy initialization
string null_book = "9-999-99999-9"; // copy initialization
string nines = string(100, '9'); // copy initialization
```

q:copy initialization是否只能使用copy constructor进行?
>No.copy initialization requires either the
copy constructor or the move constructor.

q:when copy initialization happens?
1. when we define variables using an =(注意，这里强调了是define)
2. Pass an object as an argument to a parameter of nonreference type
3. Return an object from a function that has a nonreference return type
4. Brace initialize the elements in an array or the members of an aggregate class
5. the library containers copy initialize their elements when we initialize the container, or
when we call an insert or push member(By contrast, elements
created by an emplace member are direct initialized)

#### Parameters and Return Values

q:copy constructor的参数是否必须为reference?
>Yes.If that parameter were not a reference, then the call would never succeed—to call the
copy constructor, we’d need to use the copy constructor to copy the argument, but to
copy the argument, we’d need to call the copy constructor, and so on indefinitely

#### Constraints on Copy Initialization

q:copy constructor一般是否用explicit声明?
>No.这个是我个人得到的结论，书上并没有明确说明，我觉得原因如下：
1. explicit用来修饰single argument constructor，这点是确定的，cpp-primer-7.4讲到
2. 如果explicit用来声明copy constructor，那么会和copy initialization发生场景冲突(string a = b; we define a use an =, but it's an implicit conversion, which is contrary to explicit conversion)

#### The Compiler Can Bypass the Copy Constructor

q:rt?
>During copy initialization, the compiler is permitted (but not obligated) to skip the
copy/move constructor and create the object directly. That is, the compiler is
permitted to rewrite

```cpp
string null_book = "9-999-99999-9"; // copy initialization
string null_book("9-999-99999-9"); // compiler omits the copy constructor
```

### 13.1.2. The Copy-Assignment Operator

#### Introducing Overloaded Assignment

q:assignment operators通常都是返回引用?这里的原因是什么?
>To be consistent with assignment for the built-in types (§ 4.4, p. 145), assignment
operators usually return a reference to their left-hand operand.
>
>因为需要返回值是left value，所以返回引用。内置类型都是这么做的。

```cpp
if (a = b)
  std::cout << a << std::endl;
```

q:双目运算符如果重载为成员函数，left operand是什么?
>When an operator is a member function, the left-hand operand is bound to the
implicit this parameter

#### The Synthesized Copy-Assignment Operator

q:Synthesized Copy-Assignment Operator默认行为是什么?
>Analogously to the copy constructor,
1. some classes the synthesized copy-assignment operator
disallows assignment
2. Otherwise, it assigns each nonstatic
member of the right-hand object to the corresponding member of the left-hand object
3. The synthesized copy-assignment
operator returns a reference to its left-hand object.
4. (这一点书上没写，但实际很重要)如果对象申请了资源，那么在进行赋值时，需要先释放之前的资源，再进行deep copy.

第三点是copy constructor和copy assignment operator的区别，当然这是函数体的区别。调用时机也不同。

#### 13.1.3. The Destructor

#### What a Destructor Does

q:rt?
The destructor operates inversely to the constructors: 
1. Constructors initialize the non static data members of an object 
2. and may do other work;

destructors do
1. whatever work is needed to free the resources used by an object 
2. and destroy the non static data members of the object.

所以，需要特别注意的是，构造和析构，其实都是由两部分组成的。

q:构造和析构的两部分工作具体是怎么体现的？
Just as a constructor has
1. an initialization part 
2. and a function body (§ 7.5.1, p. 288)

a destructor has
1. a function body 
2. and a destruction part.

q:构造和析构的两部分执行顺序是怎么样？
In a constructor, 
1. members are initialized 
2. before the function body is executed, and members are initialized in the same order as they appear in the class. 
In a destructor, 
1. the function body is executed first and 
2. then the members are destroyed. Members are destroyed in reverse order from the order in which they were initialized.

q:destructor function body通常用来做什么?
>The function body of a destructor does whatever operations the class designer
wishes to have executed subsequent to the last use of an object. Typically, the
destructor frees resources an object allocated during its lifetime.

q:destruction part会显示体现吗?
>In a destructor, there is nothing akin to the constructor initializer list to control how members are destroyed; the destruction part is implicit.

#### When a Destructor Is Called

The destructor is used automatically whenever an object of its type is destroyed:
- Variables are destroyed when they go out of scope.
- Members of an object are destroyed when the object of which they are a part is
destroyed.
- Elements in a container—whether a library container or an array—are destroyed
when the container is destroyed.
- Dynamically allocated objects are destroyed when the delete operator is
applied to a pointer to the object (§ 12.1.2, p. 460).
- Temporary objects are destroyed at the end of the full expression in which the
temporary was created.

我觉得这里还需要特别注意的一点是，对于static memory和dynamic memory的调用时机，前者是compiler负责，后者需要programmer负责(dynamicallly allocated object通常返回一个指针，指针对象会被编译器自动析构，但是指针所指的dynamically allocated object则不会被析构)

q:notes?
>It is important to realize that the destructor body does not directly destroy the
members themselves. Members are destroyed as part of the implicit destruction phase
that follows the destructor body. A destructor body executes in addition to the
memberwise destruction that takes place as part of destroying an object.

### 13.1.4. The Rule of Three/Five

这一小节的两个子目录就是结论。

#### Classes That Need Destructors Need Copy and Assignment

q:rt?原因是什么
>如果一个类需要一个析构函数，通常会涉及到资源分配。以内存资源分配为例，如果此时不显示定义copy constructor/copy assignment operator
那么默认的会发生浅拷贝。浅拷贝带来的问题是，如果某一个对象析构，会直接回收之前分配的内存资源。但是之前的对象并不知道，如果继续使用会出现问题，即访问已经回收的资源。

q:这里有一个我自己的问题，当我们在进行oo设计时，基类会显示定义virtual destructor，但是并没有任何资源分配的工作，那么如果按照rule of three，却需要定义copy constructor and copy operator,这个怎么处理？
>我看考了下leveldb的实现，leveldb遵循了rule of three，即使base class 没有资源相关操作

1. class DB是一个abstract class，定义interface
2. class DBImpl是一个Implementation class，进行资源管理，并实现了所有接口。由于进行资源管理，重写了destructor.

```cpp
// db.h
class LEVELDB_EXPORT DB {
 public:
  // Open the database with the specified "name".
  // Stores a pointer to a heap-allocated database in *dbptr and returns
  // OK on success.
  // Stores nullptr in *dbptr and returns a non-OK status on error.
  // Caller should delete *dbptr when it is no longer needed.
  static Status Open(const Options& options, const std::string& name,
                     DB** dbptr);

  DB() = default;

  DB(const DB&) = delete;
  DB& operator=(const DB&) = delete;

  virtual ~DB();

  // Set the database entry for "key" to "value".  Returns OK on success,
  // and a non-OK status on error.
  // Note: consider setting options.sync = true.
  virtual Status Put(const WriteOptions& options, const Slice& key,
                     const Slice& value) = 0;
  
  // ... 下面都是接口，没有任何资源管理句柄

// db_impl.h
class DBImpl : public DB {
 public:
  DBImpl(const Options& options, const std::string& dbname);

  DBImpl(const DBImpl&) = delete;
  DBImpl& operator=(const DBImpl&) = delete;

  ~DBImpl() override;

  // Implementations of the DB interface
```

#### Classes That Need Copy Need Assignment, and Vice Versa

首先，需要注意的一点是，不要搞混了。上面一小节说的是，如果我们需要一个析构函数，通常需要copy constructor and copy assigment operator=.
但是，这一个小节讲的不是反之亦成，而是说的copy constrtuctor和copy assignment operator的关系。
因为这两个函数都是copy semantics，所以通常如果需要显示定义其中一个，也需要显示定义其中另一个。但是，他们不见得需要一个析构函数。

1. 一个类需要析构函数，是需要copy constructor and copy assignment operator的充分非必要条件。
2. 一个类需要copy constructor是一个类需要copy assignment operator的充要条件。

### 13.1.5. Using = default

q:default的作用?
>We can explicitly ask the compiler to generate the synthesized versions of the copy control members by defining them as = default
>
>两点需要注意：
1. = default是一种显示定义，只不过函数体由编译器生成(以往是programmer不显示定义，然后由编译器隐式定义)
2. class内使用是inline，如果不想inline，类外使用即可

q:default的使用场景?
我的理解，= default的场景，旨在帮助programmer在必须定义class control函数的场景，但是又没有什么特殊操作，使用默认函数即可的这样一种场景。
比如，继承的时候，destructor必须显示声明为virtual，但是此时copy constructor and copy assignment operator也确实没有什么特殊的操作，如果没有= default，就需要
programmer自己显示定义一个，但是有了=default，则可以让编译器去做。

q:default的使用限制?
>We can use = default only on member functions that have a synthesized version
只有编译器会帮助生成的成员函数，才能使用。

### 实践

- demo-01

自己的实现，按照陈硕的做法，当class有资源申请的时候，禁止copy/move operation，但是同时满足rule of 5

- demo-02

按照书上的要求，进行deep copy,但是没有执行rule of 5. 因为不太知道move operation怎么写

- demo-03

修复了demo-02的bug，问题表现在class HasPtr管理资源，所以在进行赋值的时候，要先释放之前的资源，再进行deep copy.

- demo-04

课后习题，观察各种构造函数的调用时机

- demo-05

这个习题设计的非常好，有点类似与引用计数的思路。题目是这样，设计一个类，类中有一个成员(unique sequential number)，当类发生copy sematics时，进行增加。

我的思路：
1. 简单设计就是incr即可，但是不是thread-safe
2. 我又考虑能不能通过time来生成这样一个unique sequential number，显然也不对，因为thread-safe避免的就是同时访问，此时时间种子一样还是一样
3. 说到底，就是要实现thread-safe
4. 简单用Incr作为unique sequential number，但是这带来另一个问题，不同对象怎么通信

分别来进行考虑，先说thread-safe：
1. copy semantics需要考虑thread-safe
2. default constructor也需要考虑thread-safe

不同对象怎么通信：
1. 答案给了非常好的实现，用static variable
2. 不同object 共享static variable

由于此时static variable不在block scope，所以还是需要考虑锁的问题。最终的方案是这样：
1. static variable解决对象之间通信
2. static std::mutex解决thread-safe问题(不能是non-static mutex，因为这样不同thread拿到不是同一把锁，自然不会阻塞线程)

```cpp
#ifndef FOO_H_
#define FOO_H_

#include <mutex>

namespace cp {

class Foo {
 public:
  Foo() : uid_(0) {uid_ = incr();}

  Foo(const Foo&) {uid_ = incr();}
  Foo& operator=(const Foo&) {uid_ = incr(); return *this;}

  int uid() const {return uid_;}

 private:
  static int incr();

 private:
  int uid_;

  static int counter_;
  static std::mutex mtx_;
};


} // namespace cp

#endif // FOO_H_
```

参考<br>
[thread-safe-initialization-of-a-singleton)](https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton)