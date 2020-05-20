### String实现

说下我目前了解到的实现

- vector<char>的实现，本小节StrVec换成CharVec即可。这种设计非常完备且优雅
- 我的实现，我这里参考了chenshuo的实现。对于实现这个类，本质上实现copy-control.我增加了sz，来获取size的线性时间

关于代码中的一些细节，可以参考下面文档，chenshuo对于std::string有深入介绍，可以看下他的总结

参考<br>
[C++面试中STRING类的一种正确写法](https://coolshell.cn/articles/10478.html)
[StringTrival](https://github.com/chenshuo/recipes/blob/master/string/StringTrivial.h)
