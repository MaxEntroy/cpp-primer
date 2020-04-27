## Chapter13. Copy Control

### Section13.1 Copy, Assign, and Destroy

### Section13.2 Copy Control and Resource Management

### Section13.3 Swap

### Section13.4 A Copy-Control Example

### Section13.5 Classes That Manage Dynamic Memory

### Section13.6 Moving Objects

q:当我们说class control时，我们在说什么?
>When we define a class, we specify—explicitly or implicitly—what happens when objects of that class type are copied, moved, assigned, and destroyed.
A class controls these operations by defining five special member functions:
copy constructor, copy- assignment operator, move constructor, move-assignment operator, and destructor.
 Collectively, we’ll refer to these operations as copy control

q:copy/move constructor的作用是什么?
>The copy and move constructors define what happens when an object is initialized from another object of the same type

q:copy/move assignment operator的作用是什么?
> The copy- and move-assignment operators define what happens when we assign an object of a class type to another object of that same class type. 

q:destructor的作用是什么?
>The destructor defines what happens when an object of the type ceases to exist. 

**Warning**
Copy control is an essential part of defining any C++ class.