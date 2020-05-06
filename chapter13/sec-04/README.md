[TOC]

## Section13.4. A Copy-Control Example

### Basis

q:copy-control是否只在管理动态资源时需要定义
>Although copy control is most often needed for classes 
that allocate resources, resource management is not the only reason 
why a class might need to define these members. 
Some classes have bookkeeping or other actions that the copy-control members must perform.

### 实践

- demo-01

按照题目要求，我自己实现的版本。基本上和题目的标准答案一致，这个例子基本是个observer pattern的问题，
我刚开始也采用了类似的分析，不过发现还是要按照题目的考察目标来。
整个设计的思路也收到observer pattern的影响。attach/detach的做法也应用到题目中，很好的解决了问题。

我的这个版本还是更多的考虑了复用，其实设计思路也没问题，答案的办法也好，起码为swap的实现提供了灵活度。
但同时，save/remove的多余操作也不多。

所以，在设计的时候，还是在一开始，尽量考虑完整。把能想到的都想到，想不到的就算了，但是起码得有这个意识。
