## Introduction ##

* **复用的发展**：从子程序（subroutines）、程序（procedures）、函数（functions）、类别（classes），到函数库（function libraries）、类别库（class libraries）、各种组件（components）；从结构化设计、模块化设计、面向对象设计，到模式的归纳整理。
* 复用必须建立在某种标准之上－无论是语言层次的标准、数据交换的标准，或者通讯协议的标准。
* **STL所实现的，是依据泛型思维架设起来的一个概念结构。这个以抽象概念为主体而非以实际类为主体的结构，形成了一个严谨的接口标准。**
* STL六大组件：　　
    * 容器（containers）：各种数据结构
    * 算法（algorithms）：从实现的角度看，STL算法是一种function template
    * 迭代器（iterators）：扮演容器与算法之间的胶合剂。注：只有容器的设计者才能设计对应的迭代器
    * 仿函数（functors）
    * 配接器（adapters）：一种用于修饰接口的东西
    * 配置器（allocators）：负责空间配置与管理

