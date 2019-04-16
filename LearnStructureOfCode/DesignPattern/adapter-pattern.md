## 适配器模式

#### 1. 案例： 没有源码的算法库
<div align="center"><img src="pics/adapter-pattern-1.jpg" width="55%"></div>

#### 2. 适配器模式
引入了一个被称为适配器 (Adapter) 的包装类，而它所包装的对象称为适配者 (Adaptee)。
Adapter 别名为包装器 (Wrapper)。 意义： 引入新的中间人来避免对旧有代码（接口）的修改，遵循 “开闭原则”。
<div align="center"><img src="pics/adapter-pattern-2.jpg" width="55%"></div>

代码框架
```java
class Adapter extends Target {
	private Adaptee adaptee; //维持一个对适配者对象的引用
	
	public Adapter(Adaptee adaptee) {
		this.adaptee = adaptee;
	}
	
	public void request() {
		adaptee.specificRequest(); //转发调用
	}
}
```
适配器模式可分为对象适配器和类适配器两种。 在对象适配器模式中，适配器与适配者之间是关联关系；在类适配器模式中，适配器与适配者之间是继承（或实现）关系。

#### 3. 使用适配器模式来重用算法库中的算法
ScoreOperation 接口充当抽象目标，QuickSort 和 BinarySearch 类充当适配者，OperationAdapter 充当适配器。
<div align="center"><img src="pics/adapter-pattern-3.jpg" width="65%"></div>

代码框架
```java
# 1. 目标接口定义 （Interface）

// 抽象成绩操作类：目标接口
interface ScoreOperation {
	public int[] sort(int array[]); //成绩排序
	public int search(int array[],int key); //成绩查找
}

# 2. 适配者 （Adaptee）

// 快速排序类：适配者
class QuickSort {
	public int[] quickSort(int array[]) {
		sort(array,0,array.length-1);
		return array;
	}
 
	public void sort(int array[],int p, int r) {
		int q=0;
		if(p<r) {
			q = partition(array,p,r);
			sort(array,p,q-1);
			sort(array,q+1,r);
		}
	}
 
	public int partition(int[] a, int p, int r) {
		int x=a[r];
		int j=p-1;
		for (int i=p;i<=r-1;i++) {
			if (a[i]<=x) {
				j++;
				swap(a,j,i);
			}
		}
		swap(a,j+1,r);
		return j+1;	
	}
 
	public void swap(int[] a, int i, int j) {   
		int t = a[i];   
		a[i] = a[j];   
		a[j] = t;   
	}
}
 
// 二分查找类：适配者
class BinarySearch {
	public int binarySearch(int array[],int key) {
		int low = 0;
		int high = array.length -1;
		while(low <= high) {
			int mid = (low + high) / 2;
			int midVal = array[mid];
			if(midVal < key) {  
			    low = mid +1;  
			}
			else if (midVal > key) {  
			    high = mid -1;  
			}
			else {  
			    return 1; //找到元素返回1  
			}
		}
		return -1;  //未找到元素返回-1
	}
}

# 3. 适配器定义 （Adapter/Wrapper）

// 操作适配器：适配器
class OperationAdapter implements ScoreOperation {
	private QuickSort sortObj; //定义适配者QuickSort对象
	private BinarySearch searchObj; //定义适配者BinarySearch对象
 
	public OperationAdapter() {
		sortObj = new QuickSort();
		searchObj = new BinarySearch();
	}
 
	public int[] sort(int array[]) {  
		return sortObj.quickSort(array); //调用适配者类QuickSort的排序方法
	}
 
	public int search(int array[],int key) {  
		return searchObj.binarySearch(array,key); //调用适配者类BinarySearch的查找方法
	}
}

# 4. 工具类 XMLUtil 和配置文件

<?xml version="1.0"?>
<config>
	<className>OperationAdapter</className>
</config>

import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.SAXException;
import java.io.*;
class XMLUtil {
// 该方法用于从 XML 配置文件中提取具体类类名，并返回一个实例对象
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

# 5. 主程序 （main）

class Client {
	public static void main(String args[]) {
		ScoreOperation operation;  //针对抽象目标接口编程
		operation = (ScoreOperation)XMLUtil.getBean(); //读取配置文件，反射生成对象
		int scores[] = {84,76,50,69,90,91,88,96}; //定义成绩数组
		int result[];
		int score;
		
		System.out.println("成绩排序结果：");
		result = operation.sort(scores);
 
		//遍历输出成绩
		for(int i : scores) {
			System.out.print(i + ",");
		}
		System.out.println();
		
		System.out.println("查找成绩90：");
		score = operation.search(result,90);
		if (score != -1) {
			System.out.println("找到成绩90。");
		}
		else {
			System.out.println("没有找到成绩90。");
		}
		
		System.out.println("查找成绩92：");
		score = operation.search(result,92);
		if (score != -1) {
			System.out.println("找到成绩92。");
		}
		else {
			System.out.println("没有找到成绩92。");
		}
	}
}
```
说明
```
如果需要使用其他排序算法类和查找算法类，可以增加一个新的适配器类，使用新的适配器来适配新的算法，
原有代码无须修改。 符合“开闭原则”。
```

#### 4. 类适配器
类适配器模式和对象适配器模式最大的区别在于适配器和适配者之间的关系不同。 对象适配器模式中适配器和适配者之间是关联关系，而类适配器模式中适配器和适配者是继承关系。
<div align="center"><img src="pics/adapter-pattern-4.jpg" width="55%"></div>

代码框架
```java
// 适配器类实现了抽象目标类接口 Target，并继承了适配者类

class Adapter extends Adaptee implements Target {
	public void request() {
		specificRequest();
	}
}
```
特别提醒
```
由于 Java、 C# 等语言不支持多重类继承，因此类适配器的使用受到很多限制。 例如：
    ▫ 如果目标抽象类 Target 不是接口，而是一个类，就无法使用类适配器；
    ▫ 此外，如果适配者 Adapter 为最终 (Final) 类，也无法使用类适配器。
在 Java 等面向对象编程语言中，大部分情况下我们使用的是对象适配器，类适配器较少使用。
```

#### 5. 双向适配器
如果，适配器中同时包含对目标类和适配者类的引用，那么该适配器就是一个双向适配器。
<div align="center"><img src="pics/adapter-pattern-5.jpg" width="65%"></div>

代码框架
```java
class Adapter implements Target,Adaptee {
	// 同时维持对抽象目标类和适配者的引用
	private Target target;
	private Adaptee adaptee;
	
	public Adapter(Target target) {
		this.target = target;
	}
	
	public Adapter(Adaptee adaptee) {
		this.adaptee = adaptee;
	}
	
	public void request() {
		adaptee.specificRequest();
	}
	
	public void specificRequest() {
		target.request();
	}
}
```
特别提醒
```
在实际开发中，我们很少使用双向适配器。
```

#### 6. 缺省适配器 (适配器的一种变体)
当不需要实现一个接口所提供的所有方法时，可先设计一个抽象类实现该接口，并为接口中每个方法提供一个默认实现（空方法），
那么该抽象类的子类可以选择性地覆盖父类的某些方法来实现需求。 该模式又称为 “单接口适配器模式”。
<div align="center"><img src="pics/adapter-pattern-6.jpg" width="25%"></div>

代码框架
```java
# 1. 常规接口类

class ServiceInterface {
    public void serviceMethod1() {
        ……
    }

    public void serviceMethod2() {
        ……
    }

    public void serviceMethod3() {
        ……
    }
}

# 2. 缺省适配器 （常规接口类的一个空实现版本！ 用于后续的选择性适配）
（注意要使用抽象类来定义，而不能用接口类来定义）

abstract class AbstractServiceClass {
    public void serviceMethod1() { } //空实现
    public void serviceMethod2() { }
    public void serviceMethod3() { }
}

# 3. 具体类

class ConcreteServiceClass extends AbstractServiceClass {
    public void serviceMethod1() {
        ……
    }
}
```
说明
```
ServiceInterface - 接口 （API 集合）
AbstractServiceClass - 缺省适配器 （为接口中每个方法提供一个默认实现（空方法））
意义/作用：
    在 Java 语言中，一般我们可以使用两种方式来实现窗口事件处理类: 1. 通过实现 WindowListener 接口，2. 通过继承 WindowAdapter 默认适配器类。
    如果是使用第一种方式，直接实现 WindowListener 接口，事件处理类需要实现在该接口中定义的七个方法，
    而对于大部分需求可能只需要实现一两个方法，其他方法都无须实现，但由于语言特性我们又不得不为其他方法也提供一个实现。
    （此时，继承默认适配器就显得方便很多。）
```

#### 7. 本模式优缺点
```
优：
▪ 实现了对现有类的复用，同时符合“开闭原则”。
缺：
类适配器模式
    ▪ 对于不支持多重类继承的语言，一次最多只能适配一个适配者类。
    ▪ 适配者类不能为最终类。
    ▪ 在 Java、 C# 等语言中，目标抽象类只能为接口，不能为类。
对象适配器模式
    ▪ 在适配器中置换适配者类的某些方法比较麻烦。
```
适用场景
```
▫ 关联现有类
▫ 类的接口（如方法名）不符合系统的需要，或者这些类没有源代码。
```
