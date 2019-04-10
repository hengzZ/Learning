## 抽象工厂模式
当前处境：
```
1. 工厂方法模式通过引入工厂等级结构，解决了简单工厂模式中静态工厂类职责太重的问题。
2. 由于工厂方法模式中的每个工厂只生产一类产品，导致系统中存在大量的工厂类。
```
新需求：
```
降低大型系统中存在的工厂类数量。
```

抽象工厂模式的基本思想是，将一些相关的产品组成一个“产品族”，由同一个工厂来统一生产。 （产品工厂 -> 套件工厂）

#### 1. 案例： 界面皮肤库
<div align="center"><img src="pics/abstract-factory-pattern-1.jpg" width="45%"></div>
<div align="center"><img src="pics/abstract-factory-pattern-2.jpg" width="70%"></div>
分析：

```
以上方案，提供了大量工厂来创建具体的界面组件，拥有广泛的组合方式。
存在的弊端：
    ▪ 每增加一个具体组件都需要增加一个具体工厂，使原本复杂的系统更加复杂和庞大。
     （因为系统天生的复杂特性才对扩展性要求高）
    ▪ 通常同一种风格的具体界面组件要一起显示，即对应一组具体工厂。时刻需要牢记一种套件（组合）所依赖的具体工厂列表。
     （类似于衣服穿搭）
```

#### 2. 解决思路： 产品等级结构与产品族
```
(1) 产品等级结构：产品等级结构即产品的继承结构，如一个抽象类是电视机，其子类有海尔电视机、海信电视机、TCL电视机。
    抽象电视机与具体品牌的电视机之间构成了一个产品等级结构。
(2) 产品族：在抽象工厂模式中，产品族是指由同一个工厂生产的，位于不同产品等级结构中的一组产品。
    如海尔电器工厂生产的海尔电视机、海尔电冰箱。
```

#### 3. 抽象工厂模式
抽象工厂模式提供一个创建一系列相关或相互依赖对象的接口。抽象工厂模式又称为 Kit 模式。
<div align="center"><img src="pics/abstract-factory-pattern-3.jpg" width="70%"></div>

##### 代码框架
```java
抽象工厂可以是接口，也可以是抽象类或者具体类。

# 1. 抽象工厂定义 （包含多种工厂方法）

abstract class AbstractFactory {
    public abstract AbstractProductA createProductA(); //工厂方法一
    public abstract AbstractProductB createProductB(); //工厂方法二
    ……
}

# 2. 具体工厂定义

class ConcreteFactory1 extends AbstractFactory {
    //工厂方法一
    public AbstractProductA createProductA() {
        return new ConcreteProductA1();
    }
 
    //工厂方法二
    public AbstractProductB createProductB() {
        return new ConcreteProductB1();
    }
 
    ……
}

# 3. 主程序 （main）

……
AbstractFactory factory;
factory = new ConcreteFactory1(); //可通过配置文件实现
AbstractProductA productA;
AbstractProductB productB;
productA = factory.createProductA();
productB = factory.createProductB();
……
```

#### 4. 重构
使用抽象工厂模式来重构界面皮肤库的设计。 （产品族）
<div align="center"><img src="pics/abstract-factory-pattern-4.jpg" width="70%"></div>

代码框架：
```java
# 1. 抽象产品定义

//按钮接口：抽象产品
interface Button {
	public void display();
}

//文本框接口：抽象产品
interface TextField {
	public void display();
}

//组合框接口：抽象产品
interface ComboBox {
	public void display();
}

# 2. 具体产品定义

//Spring按钮类：具体产品
class SpringButton implements Button {
	public void display() {
		System.out.println("显示浅绿色按钮。");
	}
}
 
//Summer按钮类：具体产品
class SummerButton implements Button {
	public void display() {
		System.out.println("显示浅蓝色按钮。");
	}	
}

//Spring文本框类：具体产品
class SpringTextField implements TextField {
	public void display() {
		System.out.println("显示绿色边框文本框。");
	}
}
 
//Summer文本框类：具体产品
class SummerTextField implements TextField {
	public void display() {
		System.out.println("显示蓝色边框文本框。");
	}	
}

//Spring组合框类：具体产品
class SpringComboBox implements ComboBox {
	public void display() {
		System.out.println("显示绿色边框组合框。");
	}
}
 
//Summer组合框类：具体产品
class SummerComboBox implements ComboBox {
	public void display() {
		System.out.println("显示蓝色边框组合框。");
	}	
}

# 3. 抽象工厂定义

//界面皮肤工厂接口：抽象工厂
interface SkinFactory {
	public Button createButton();
	public TextField createTextField();
	public ComboBox createComboBox();
}

# 4. 具体工厂定义

//Spring皮肤工厂：具体工厂
class SpringSkinFactory implements SkinFactory {
	public Button createButton() {
		return new SpringButton();
	}
 
	public TextField createTextField() {
		return new SpringTextField();
	}
 
	public ComboBox createComboBox() {
		return new SpringComboBox();
	}
}
 
//Summer皮肤工厂：具体工厂
class SummerSkinFactory implements SkinFactory {
	public Button createButton() {
		return new SummerButton();
	}
 
	public TextField createTextField() {
		return new SummerTextField();
	}
 
	public ComboBox createComboBox() {
		return new SummerComboBox();
	}
}

# 5. 主程序 （main）

class Client {
	public static void main(String args[]) {
        //使用抽象层定义
		SkinFactory factory;
		Button bt;
		TextField tf;
		ComboBox cb;
		factory = (SkinFactory)XMLUtil.getBean();
		bt = factory.createButton();
		tf = factory.createTextField();
		cb = factory.createComboBox();
		bt.display();
		tf.display();
		cb.display();
	}
}
```

工具类 XMLUtil 和 配置文件:
```java
<?xml version="1.0"?>
<config>
	<className>SpringSkinFactory</className>
</config>
```
```java
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.SAXException;
import java.io.*;
 
public class XMLUtil {
//该方法用于从XML配置文件中提取具体类类名，并返回一个实例对象
	public static Object getBean() {
		try {
			//创建文档对象
			DocumentBuilderFactory dFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder builder = dFactory.newDocumentBuilder();
			Document doc;							
			doc = builder.parse(new File("config.xml")); 
		
			//获取包含类名的文本节点
			NodeList nl = doc.getElementsByTagName("className");
			Node classNode=nl.item(0).getFirstChild();
			String cName=classNode.getNodeValue();
            
			//通过类名生成实例对象并将其返回
			Class c=Class.forName(cName);
			Object obj=c.newInstance();
			return obj;
		}
		catch(Exception e) {
			e.printStackTrace();
			return null;
		}
	}
}
```

进一步改进：
```
在真实项目开发中，我们通常会为配置文件提供一个可视化的编辑界面，类似 Struts 框架中的 struts.xml 编辑器，
实现真正的纯界面操作。
```

#### 5. 思考 （没有绝对完美的方案）
由于设计时考虑不全面，忘记为单选按钮 (Radio Button) 提供不同皮肤的风格化显示。 现在发现，原有系统居然不能够在符合“开闭原则”的前提下增加新的组件。
Amazing !!!
```
答案是抽象工厂模式无法解决该问题，这也是抽象工厂模式最大的缺点。
在抽象工厂模式中，增加新的产品族很方便，但是增加新的产品等级结构很麻烦。需要修改所有现存工厂。
（当前这种特点称为，“开闭原则”的倾斜性。）

在这种情况下，工厂方法模式更有效，也可满足“开闭原则”。
```

#### 6. 本模式优缺点
抽象工厂模式是工厂方法模式的进一步延伸，带来一定好处的同时，带来“开闭原则”的倾斜性。
```
优：
▪ 隔离了具体类的生成细节。
▪ 更符合产品族的管理。 （套装/套餐场景下，减少对产品组合列表的记忆维护）
缺：
▪ 只能基于已有产品族的修改。
▪ 增加新的产品等级结构麻烦，需要对原有系统进行较大的修改。 从抽象工厂到所有具体工厂。
```
适用场景
```
要求设计人员在设计之初就能够全面考虑，不会在设计完成之后向系统中增加新的产品等级结构，
也不会删除已有的产品等级结构，否则将会导致系统出现较大的修改。
```
