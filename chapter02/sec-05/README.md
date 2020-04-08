[TOC]
## Section2.5 Dealing with Types

### 2.5.1 Type Alias

#### basic

q:什么是type alias
>A type alias is a name that is a synonym for another type.

q:type alias的优点是什么？
>Type aliases let us simplify complicated type definitions, making those types easier to use. Type aliases also let us emphasize the purpose for which a type is used.

q:type alias的语法是什么(如何定义一个type alias)?
- Traditionally, we use a typedef
- The new standard introduced a second way to define a type alias, via an alias declaration(using type_alias = original type)

#### Pointers, const, and Type Aliases

### 2.5.2 The auto Type Specifier

#### basic

q:为什么要增加这样一个关键字?
- It is not uncommon to want to store the value of an expression in a variable.
- To declare the variable, we have to know the type of that expression
- When we write a program, it can be surprisingly difficult—and sometimes even impossible—to determine the type of an expression
>简言之，有时候会存在表达式的类型难以判断的情况，这会导致left value的类型也难以判断。
所以，新的标准为我们提供了这样一个关键字。
Under the new standard, we can let the compiler figure out the type for us by using the auto type specifier

q:auto进行type deduce的根据是什么?
>auto tells the compiler to deduce the type from the initializer

#### Compound Types, const, and auto

### 2.5.3 The decltype Type Specifier

q:既然有了auto来帮助我们通过表达式来进行类型推断，那么decltype还有什么用？
>Sometimes we want to define a variable with a type that the compiler deduces from an expression but do not want to use that expression to initialize the variable.
>
>简言之，想用表达式类做类型type deduce，但是又不想用表达式的值来初始化左值。
>The compiler analyzes the expression to determine its type but does not evaluate the expression.