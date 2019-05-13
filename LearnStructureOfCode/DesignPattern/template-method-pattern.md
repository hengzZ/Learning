## 模板方法模式
“基于继承的代码复用技术。”
```
“钩子方法” 模式。
```

#### 1. 模板方法

##### 1.1 核心
```
将复杂流程的实现步骤，封装在一系列基本方法中， （将复杂的处理工艺分解到不同工位上的工艺）
在抽象父类中提供一个称之为 “模板方法” 的方法来定义这些基本方法的执行次序。 （基于基本处理函数编程。）
```
“模板方法”基于基本处理函数进行编程。
<div align="center"><img src="pics/template-method-pattern-1.jpg" width="45%"></div>

##### 1.2 代码框架
实现具体逻辑步骤的方法为 “基本方法”，将基本方法汇总起来的方法为 “模板方法”。
* 模板方法定义在抽象类中，明确定义了由基本操作方法组合的一个总行为，并由子类不加以修改地完全继承下来。
* 基本方法是实现算法/行为的各个步骤的方法，可分为三种： 抽象方法 (Abstract Method)、 具体方法 (Concrete Method) 和 钩子方法 (Hook Method)。

##### 第一类钩子方法： IsXXX()。
“用于对某个条件进行判断，如果条件满足则执行某一步骤，否则将不执行。”

代码框架
```java
//模板方法
public void TemplateMethod() 
{
	Open();
	Display();
	//通过钩子方法来确定某步骤是否执行
	if (IsPrint()) {
		Print();
	}
}
 
//钩子方法
public bool IsPrint()
{
	return true;
}
```

##### 第二类钩子方法： 实现体为空的具体方法。 （最常使用的“钩子”方式）

代码框架
```java
# 抽象父类

abstract class AbstractClass 
{
	//模板方法
	public void TemplateMethod() 
	{
		PrimitiveOperation1();
		PrimitiveOperation2();
		PrimitiveOperation3();  //“钩子”
	}
 
	//基本方法—具体方法
	public void PrimitiveOperation1() 
	{
		//实现代码
	}
 
	//基本方法—抽象方法
	public abstract void PrimitiveOperation2();    
 
	//基本方法—钩子方法
	public virtual void PrimitiveOperation3()   
	{ /* 钩子方法必须是提供一个空实现 */ }
}

# 子类

class ConcreteClass : AbstractClass 
{
	public override void PrimitiveOperation2() 
	{
		//实现代码
	}
 
	public override void PrimitiveOperation3() 
	{
		//实现代码
	}
}
```

##### 1.3 模板方法的执行
程序在运行时，具体子类的基本方法将覆盖父类中定义的基本方法，子类的钩子方法也将覆盖父类的钩子方法，从而可以通过在子类中实现的钩子方法对父类方法的执行进行约束，实现子类对父类行为的反向控制。

#### 2. 案例： 银行系统的利息计算模块
利息计算流程：
* 验证用户信息，如果用户信息错误，系统显示出错提示。
* 根据用户类型的不同使用不同的利息计算公式计算利息。
* 显示利息。

（基本流程/模板方法分为三个基本操作/基本方法： 验证基本处理函数、 计算基本处理函数、 显示基本处理函数。）

<div align="center"><img src="pics/template-method-pattern-2.jpg" width="45%"></div>
Account 充当抽象类角色，CurrentAccount 和 SavingAccount 充当具体子类角色。

代码框架
```java
# 1. 抽象父类

//抽象账户类
abstract class Account
{
	//基本方法——具体方法
	public bool Validate(string account, string password) 
	{
		Console.WriteLine("账号：{0}", account);
		Console.WriteLine("密码：{0}", password);
		//模拟登录
		if (account.Equals("张无忌") && password.Equals("123456")) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	//基本方法——抽象方法
	public abstract void CalculateInterest();

	//基本方法——具体方法
	public void Display() 
	{
		Console.WriteLine("显示利息！");
	}

	//模板方法
	public void Handle(string account, string password) 
	{
		if (!Validate(account,password)) 
		{
			Console.WriteLine("账户或密码错误！");
			return;
		}
		CalculateInterest();
		Display();
	}
}

# 2. 子类

//活期账户
class CurrentAccount : Account
{
	//覆盖父类的抽象基本方法
	public override void CalculateInterest() 
	{
		Console.WriteLine("按活期利率计算利息！");
	}
}

//定期账户
class SavingAccount : Account
{
	//覆盖父类的抽象基本方法
	public override void CalculateInterest() 
	{
		Console.WriteLine("按定期利率计算利息！");
	}
}

# 3. 主程序 （main）

<?xml version="1.0" encoding="utf-8" ?>
<configuration>
    <appSettings>
        <add key="subClass" value="TemplateMethodSample.CurrentAccount"/>
    </appSettings>
</configuration>

using System;
using System.Configuration;
using System.Reflection;
 
namespace TemplateMethodSample
{
	class Program
	{
		static void Main(string[] args)
		{
			Account account;
			//读取配置文件
			string subClassStr = ConfigurationManager.AppSettings["subClass"];
			//反射生成对象
			account = (Account)Assembly.Load("TemplateMethodSample").CreateInstance(subClassStr);
			account.Handle("张无忌", "123456");
			Console.Read();
		}
	}
}
```

#### 3. 钩子方法的使用
“包含钩子方法的模式一定是应用了模板方法模式。”

##### 3.1 钩子方法
模板方法模式，在父类中的模板方法内定义一个算法框架，同时为继承类提供一系列抽象方法、具体方法和钩子方法。

* 最简单的钩子方法就是空方法，当然也可以在钩子方法中定义一个默认的实现。
```java
比如：
public void TemplateMethod()
{
    ......
    Display();  //钩子用于具体行为
    ......
}

public virtual void Display() {}
```

* 另一种钩子方法可以实现对其他方法进行约束，此类钩子为返回 boolean 类型的 IsXXX() 函数。
```java
比如：
public void TemplateMethod()
{
    ......
    if (IsPrint()) {
        Print();  //钩子用于分支决策
    }
    ......
} 

public bool IsPrint()
{
    return true;
}
```

#### 4. 本模式优缺点
模板方法模式是一种使用较为频繁的模式，广泛应用于框架设计中，以确保通过父类来控制处理流程的逻辑顺序（如框架的初始化，测试流程的设置等）。
```
优：
▪ 在父类中形式化地定义一个算法，在执行步骤层次明确行为逻辑，由它的子类来实现细节的处理。
▪ 作为一种代码复用技术，在类库设计中尤为重要。 （“上层建设” 与 “底层实现” 的完美结合。）
缺：
▪ 需要为每一个基本方法的不同实现提供一个子类。 
（如果父类中可变的基本方法太多，可结合 “桥接模式” 来进行设计。）
```
适用场景
```
▫ 想使用 “钩子方法” 的时候。 （其实所有的基本方法都是钩子。）
```
