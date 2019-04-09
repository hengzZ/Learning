## 简单工厂模式

工厂模式是最常用的一类创建型设计模式，通常我们所说的工厂模式是**指工厂方法模式**，它也是使用频率最高的工厂模式。
此处的简单工厂模式是工厂方法模式的“小弟”，它不属于 GoF 23 种设计模式，但在软件开发中应用也较为频繁，通常将它作为学习其他工厂模式的入门。
此外，工厂方法模式还有一位“大哥”—抽象工厂模式。这三种工厂模式各具特色，难度也逐个加大，在软件开发中它们都得到了广泛的应用，成为面向对象软件中常用的创建对象的工具。

#### 1. 案例： 图表库的设计
基于 Java 语言开发一套图表库，该图表库可以为应用系统提供各种不同外观的图表，例如柱状图、饼状图、折线图等。
而且可以较为方便地对图表库进行扩展。

```java
class Chart {
	private String type; //图表类型
	
	public Chart(Object[][] data, String type) {
		this.type = type;
		if (type.equalsIgnoreCase("histogram")) {
			//初始化柱状图
		}
		else if (type.equalsIgnoreCase("pie")) {
			//初始化饼状图
		}
		else if (type.equalsIgnoreCase("line")) {
			//初始化折线图
		}
	}
 
	public void display() {
		if (this.type.equalsIgnoreCase("histogram")) {
			//显示柱状图
		}
		else if (this.type.equalsIgnoreCase("pie")) {
			//显示饼状图
		}
		else if (this.type.equalsIgnoreCase("line")) {
			//显示折线图
		}	
	}
}
```
分析：
```
该类的设计中存在如下几个问题 （Chart 类是一个“巨大的”类）：
    1. 包含很多 “if…else…” 代码块，整个类的代码相当冗长。
    2. 职责过重，负责初始化和显示所有的图表对象，将各种图表对象的初始化代码和显示代码集中在一个类中实现，违反了“单一职责原则”。
    3. 需要增加新类型的图表时，必须修改Chart类的源代码，违反了“开闭原则”。
    4. 只能通过 new 关键字来直接创建 Chart 对象，Chart 类与客户端类耦合度较高。（违反依赖倒转原则）
    5. 创建 Chart 对象之前可能还需要进行大量初始化设置，每种图表的设置项又不尽相同。
```

#### 2. 解决思路： 简单工厂模式
首先将需要创建的各种不同对象（例如各种不同的 Chart 对象）的相关代码封装到不同的类中，这些类称为具体产品类，而将它们公共的代码进行抽象和提取后封装在一个抽象产品类中，每一个具体产品类都是抽象产品类的子类；
然后提供一个工厂类用于创建各种产品，在工厂类中提供一个创建产品的工厂方法，该方法可以根据所传入的参数不同创建不同的具体产品对象；客户端只需调用工厂类的工厂方法并传入相应的参数即可得到一个产品对象。
```
1. 公共代码（相同部分） -> 抽象类部分
2. 差异化内容         -> 具体类部分
3. 一个工厂类 + 多个工厂方法
```

##### 2.1 特征
```
在简单工厂模式中用于创建实例的方法是静态 (static) 方法，因此简单工厂模式又被称为静态工厂方法 (Static Factory Method) 模式。
```
核心： 工厂类的设计 (包含多个 static 方法)<br>
<div align="center"><img src="pics/simple-factory-pattern-1.jpg" width="55%"></div>

##### 2.2 代码框架
###### 使用前提： 不能设计一个包罗万象的产品类，而是根据实际情况设计一个产品层次结构，将所有产品类公共的代码移至抽象产品类，并在抽象产品类中声明一些抽象方法，以供不同的具体产品类来实现。
```java
# 1. 产品抽象类定义

abstract class Product {
    //所有产品类的公共业务方法
    public void methodSame() {
        //公共方法的实现
    }
 
    //声明抽象业务方法
    public abstract void methodDiff();
}

# 2. 具体类定义

class ConcreteProduct extends Product {
    //实现业务方法
    public void methodDiff() {
        //业务方法的实现
    }
}

# 3. 工厂类（ static 工厂方法）定义

class Factory {
    //静态工厂方法
	public static Product getProduct(String arg) {
		Product product = null;
		if (arg.equalsIgnoreCase("A")) {
			product = new ConcreteProductA();
            //初始化设置product
		}
		else if (arg.equalsIgnoreCase("B")) {
			product = new ConcreteProductB();
            //初始化设置product
		}
		return product;
	}
}

# 4. 主程序（main）

class Client {
	public static void main(String args[]) {
		Product product; 
		product = Factory.getProduct("A"); //通过工厂类创建产品对象
		product.methodSame();
		product.methodDiff();
	}
}
```

#### 3. 重构
将 Chart 类的职责分离，由此将 Chart 对象的创建和使用分离。 重构图如下：
<div align="center"><img src="pics/simple-factory-pattern-2.jpg" width="60%"></div>

代码框架：
```java
# 1. 抽象产品类定义

//抽象图表接口：抽象产品类
interface Chart {
	public void display();
}

# 2. 具体类定义

//柱状图类：具体产品类
class HistogramChart implements Chart {
	public HistogramChart() {
		System.out.println("创建柱状图！");
	}
	
	public void display() {
		System.out.println("显示柱状图！");
	}
}

//饼状图类：具体产品类
class PieChart implements Chart {
	public PieChart() {
		System.out.println("创建饼状图！");
	}
	
	public void display() {
		System.out.println("显示饼状图！");
	}
}
 
//折线图类：具体产品类
class LineChart implements Chart {
	public LineChart() {
		System.out.println("创建折线图！");
	}
	
	public void display() {
		System.out.println("显示折线图！");
	}
}

# 3. 工厂类（ static 工厂方法）定义

//图表工厂类：工厂类
class ChartFactory {
    //静态工厂方法
	public static Chart getChart(String type) {
		Chart chart = null;
		if (type.equalsIgnoreCase("histogram")) {
			chart = new HistogramChart();
			System.out.println("初始化设置柱状图！");
		}
		else if (type.equalsIgnoreCase("pie")) {
			chart = new PieChart();
			System.out.println("初始化设置饼状图！");
		}
		else if (type.equalsIgnoreCase("line")) {
			chart = new LineChart();
			System.out.println("初始化设置折线图！");			
		}
		return chart;
	}
}

# 4. 主程序 （main）

class Client {
	public static void main(String args[]) {
		Chart chart;
		chart = ChartFactory.getChart("histogram"); //通过静态工厂方法创建产品
		chart.display();
	}
}
```

#### 4. 进一步改进 （以满足开闭原则）
当前存在的问题：
```
    每更换一个 Chart 对象都需要修改客户端代码中静态工厂方法的参数，客户端代码将要重新编译。
    这对于客户端代码而言，违反了 “开闭原则” 。
```

解决方案：
```
配置文件 + 配置文件工具类
```
将静态工厂方法的参数存储在 XML 或 properties 格式的配置文件中，如下 config.xml 所示。
```java
<?xml version="1.0"?>
<config>
	<chartType>histogram</chartType>
</config>
```

##### 客户端代码重构
```java
# 1. 配置文件工具类 XMLUtil

import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.SAXException;
import java.io.*;
 
public class XMLUtil {
    //该方法用于从 XML 配置文件中提取图表类型，并返回类型名
    public static String getChartType() {
        try {
            //创建 xml 文档对象
            DocumentBuilderFactory dFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = dFactory.newDocumentBuilder();
            Document doc;							
            doc = builder.parse(new File("config.xml")); 
            
            //获取包含图表类型的文本节点
            NodeList nl = doc.getElementsByTagName("chartType");
            Node classNode = nl.item(0).getFirstChild();
            String chartType = classNode.getNodeValue().trim();
            return chartType;
        }   
       	catch(Exception e) {
           	e.printStackTrace();
        	return null;
        }
    }
}

# 2. 主程序 （main）

class Client {
	public static void main(String args[]) {
		Chart chart;
		String type = XMLUtil.getChartType(); //读取配置文件中的参数
		chart = ChartFactory.getChart(type); //创建产品对象
		chart.display();
	}
}
```

#### 5. 遗留问题 （没有绝对完美的方案）
```
在简单工厂模式中增加新的具体产品时是否符合“开闭原则”？ 如果不符合，原有系统需作出哪些修改？
```

#### 6. 变形形式
有时候，为了简化简单工厂模式。 可以将抽象产品类和工厂类合并，将静态工厂方法移至抽象产品类中。

#### 7. 本模式优缺点
```
优：
▪ 对象的创建和对象的使用分离。（但是，工厂类包含必要的类型判断逻辑。）
▪ 引入配置文件，可以在不修改任何客户端代码的情况下更换具体类。

缺：
▪ 工厂类集中了所有产品的创建逻辑，职责过重。
▪ 增加系统中类的个数（引入了一个新的工厂类），增加了系统的复杂度和理解难度。
▪ 扩展困难，一旦添加新产品就不得不修改工厂逻辑。
▪ 静态工厂方法的存在，造成工厂角色（工厂类）无法形成基于继承的等级结构。
```
适用场景
```
具体类的种类较少，不会造成工厂方法中的业务逻辑太过复杂。
```
