[TOC]
## Section 7.3 Additional Class Features

### 7.3.1 Class Members Revisited

#### Basic

#### mutable Data Members

q:definition?
1. It sometimes (but not very often) happens that a class has a data member that we
want to be able to modify, even inside a const member function. We indicate such
members by including the mutable keyword in their declaration.
2. A mutable data member is never const, even when it is a member of a const
object.Accordingly, a const member function may change a mutable member.

q:如何理解？
>对于变量，和常量语义都很好理解。mutable显然不能修饰常量，只能修饰变量。可是既然都是变量，本来就可以变化，为什么还需要mutable修饰？
事实上，mutable 是用来修饰一个 const 示例的部分可变的数据成员的。如果要说得更清晰一点，就是说 mutable 的出现，将 C++ 中的 const 的概念分成了两种。
这里要特别注意前提，mutable只能修饰类成员。
引入 mutable 之后，C++ 可以有逻辑层面的 const，也就是对一个常量实例来说，从外部观察，它是常量而不可修改；但是内部可以有非常量的状态。
这里其实主要针对的是const member function而言。下面我们来看一个classical use

```cpp
class HashTable {
 public:
    //...
    std::string lookup(const std::string& key) const
    {
        if (key == last_key_) {
            return last_value_;
        }

        std::string value{this->lookupInternal(key)};

        last_key_   = key;
        last_value_ = value;

        return value;
    }

 private:
    mutable std::string last_key_
    mutable std::string last_value_;
};
```

我们来分析下上面的代码，
1. lookup只是做查询操作，显然不会改变内部对象的状态，所以const语义没有问题
2. 但是，由于每次都需要查询，所以我们希望能增加一个缓存，来提升性能。这也是非常常规的想法
3. 问题来了，由于缓存在lookup函数中需要被更新，那么它一定不能是常量。但是，这又是const function，参数本质是const this，即成员是const语义，不能修改
4. 此时，mutable的作用发挥。A mutable data member is never const, even when it is a member of a const
object.

那么我们来看mutable的好处是什么：
1. 首先，上面lookup函数的const语义没有变化。对于client来说，从外部观察确实是常量语义
2. 其次，mutable提供了常量内部的非常量状态。当然，这个状态不应该影响整个接口的语义，而是起到了辅助实现常语义的功能。

q:best practice?
1. const member function中mutable变量起到辅助作用，来帮助实现整体接口的常量语义
2. 常见于caching, mutex等辅助变量/结构

ps:effective modern cpp item 16，也给了非常好的例子

参考
[C++ 中的 mutable 关键字](https://liam.page/2017/05/25/the-mutable-keyword-in-Cxx/)