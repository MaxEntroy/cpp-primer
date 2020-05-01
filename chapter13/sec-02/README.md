[TOC]

## Section13.2 Copy Control and Resource Management

### 实践

- demo-01

自己实现了value-like版本

- demo-02

根据书上的要求，修正了demo-01的问题，主要是两个方面
1. 自赋值的问题需要修复
2. 析构left operand和复制right operand的顺序

- demo-03

自己实现了pointer-like版本，这里和书上的建议实现一致，采用smart pointer管理需要共享语义的资源
