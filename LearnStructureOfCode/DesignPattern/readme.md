# Design Pattern
7 个面向对象设计原则和 24 个设计模式 （23个GoF设计模式 + 简单工厂模式）

模式 - 每个模式都描述了一个在我们的环境中不断出现的问题，然后描述了该问题的解决方案的核心。（Solution）

## Contents

**面向对象设计原则**
###### 如何同时提高一个软件系统的可维护性和可复用性是面向对象设计需要解决的核心问题之一。
```
▪ 单一职责原则 (Single Responsibility Principle, SRP)
    一个类只负责一个功能领域中的相应职责。
▪ 开闭原则 (Open-Closed Principle, OCP)
    软件实体应对扩展开放，而对修改关闭。
▪ 里氏代换原则 (Liskov Substitution Principle, LSP)
    所有引用基类对象的地方能够透明地使用其子类的对象。
▪ 依赖倒转原则 (Dependence  Inversion Principle, DIP)
    抽象不应该依赖于细节，细节应该依赖于抽象。
▪ 接口隔离原则 (Interface Segregation Principle, ISP)
    使用多个专门的接口，而不使用单一的总接口。
▪ 合成复用原则 (Composite Reuse Principle, CRP)
    尽量使用对象组合，而不是继承来达到复用的目的。
▪ 迪米特法则 (Law of Demeter, LoD)
    一个软件实体应当尽可能少地与其他实体发生相互作用
```

<br>

**创造型模式**
```
1. 简单工厂模式 （Simple Factory Pattern）    （难度：2 频率：3）
2. 工厂方法模式 （Factory Method Pattern）    （难度：2 频率：5）
3. 抽象工厂模式 （Abstract Factory Pattern）  （难度：4 频率：5）
4. 单例模式 （Singleton Pattern）             （难度：1 频率：4）
5. 原型模式 （Prototype Pattern）             （难度：3 频率：3）
6. 建造者模式 （Builder Pattern）             （难度：4 频率：2）
```

**结构型模式**
```
1. 适配器模式 （Adapter Pattern）      （难度：2 频率：4）
2. 桥接模式   （Bridge Pattern）       （难度：3 频率：3）
3. 组合模式   （Composite Pattern）    （难度：3 频率：4）
4. 装饰模式   （Decorator Pattern）    （难度：3 频率：3）
5. 外观模式   （Facade Pattern）       （难度：1 频率：5）
6. 享元模式   （Flyweight Pattern）    （难度：4 频率：1）
7. 代理模式   (Proxy Pattern)          （难度：3 频率：4）
```

**行为型模式**
```
1. 职责链模式 （Chain of Responsibility Pattern）   （难度：3 频率：2）
2. 命令模式   （Command Pattern）                   （难度：3 频率：4）
3. 解释器模式 （Interpreter Pattern）               （难度：5 频率：1）
4. 迭代器模式 （Iterator Pattern）                  （难度：3 频率：5）
5. 中介者模式 （Mediator Pattern）                  （难度：3 频率：2）
6. 备忘录模式 （Memento Pattern）                   （难度：2 频率：2）
7. 观察者模式 （Observer Pattern）                  （难度：3 频率：5）
8. 状态模式   （State Pattern）                     （难度：3 频率：3）
9. 策略模式   （Strategy Pattern）                  （难度：1 频率：4）
10. 模板方法模式 （Template Method Pattern）        （难度：2 频率：3）
11. 访问者模式   （Visitor Patter）                 （难度：4 频率：1）
```

#### 目录

##### 创造型模式
1. 简单工厂模式 [simple-factory-pattern.md](simple-factory-pattern.md)
2. 工厂方法模式 [factory-method-pattern.md](factory-method-pattern.md)
3. 抽象工厂模式 [abstract-factory-pattern.md](abstract-factory-pattern.md)
4. 单例模式 [singleton-pattern.md](singleton-pattern.md)
5. 原型模式 [prototype-pattern.md](prototype-pattern.md)
6. 建造者模式 [builder-pattern.md](builder-pattern.md)

##### 结构型模式
1. 适配器模式 [adapter-pattern.md](adapter-pattern.md)
2. 桥接模式 [bridge-pattern.md](bridge-pattern.md)
3. 组合模式 [composite-pattern.md](composite-pattern.md)
4. 装饰模式 [decorator-pattern.md](decorator-pattern.md)
5. 外观模式 [facade-pattern.md](facade-pattern.md)
6. 享元模式 [flyweight-pattern.md](flyweight-pattern.md)
7. 代理模式 [proxy-pattern.md](proxy-pattern.md)

##### 行为型模式
1. 职责链模式 [chain-of-responsibility-pattern.md](chain-of-responsibility-pattern.md)
2. 命令模式 [command-pattern.md](command-pattern.md)
3. 解释器模式 [interpreter-pattern.md](interpreter-pattern.md)
4. 迭代器模式 [iterator-pattern.md](iterator-pattern.md)
5. 中介者模式 [mediator-pattern.md](mediator-pattern.md)
6. 备忘录模式 [memento-pattern.md](memento-pattern.md)
7. 观察者模式 [observer-pattern.md](observer-pattern.md)
8. 状态模式 [state-pattern.md](state-pattern.md)
9. 策略模式 [strategy-pattern.md](strategy-pattern.md)
10. 模板方法模式 [template-method-pattern.md](template-method-pattern.md)
11. 访问者模式 [visitor-pattern.md](visitor-pattern.md)

<br>

### **绪论**
Java、C#、C++ 等编程语言，Eclipse、Visual Studio 等开发工具，JSP、ASP.net 等开发技术，Struts、Hibernate、JBPM 等框架技术，所有这些我们都可以认为是招式；而数据结构、算法、设计模式、重构、软件工程等则为内功。

* 设计模式从何而来

每个模式都描述了一个在我们的环境中不断出现的问题，然后描述了该问题的解决方案的核心，通过这种方式，我们可以无数次地重用那些已有的成功的解决方案，无须再重复相同的工作。

软件模式并非仅限于设计模式，还包括架构模式、分析模式和过程模式等，实际上，在软件开发生命周期的每一个阶段都存在着一些被认同的模式。

```
软件模式的基础结构主要由四部分构成：
    ▪ 问题描述 （待解决的问题是什么）
    ▪ 前提条件 （在何种环境或约束条件下使用）
    ▪ 解法 （如何解决）
    ▪ 效果 （有哪些优缺点）
```
```
设计模式可分为:
    ▪ 创建型(Creational)
       创建型模式主要用于描述如何创建对象。
    ▪ 结构型(Structural)
       结构型模式主要用于描述如何实现类或对象的组合。
    ▪ 行为型(Behavioral)
       行为型模式主要用于描述类或对象怎样交互以及怎样分配职责。
根据某个模式主要是用于处理类之间的关系还是对象之间的关系，设计模式还可以分为：
    ▪ 类模式
    ▪ 对象模式
```

<br>

###### 模式从不保证任何东西，它不能保证你一定能够做出可复用的软件，提高你的生产率，更不能保证世界和平，微笑。模式并不能替代人来完成软件系统的创造，它只不过会给那些缺乏经验但却具备才能和创造力的人带来希望。
