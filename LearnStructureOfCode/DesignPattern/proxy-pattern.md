## 代理模式
由于某些原因，客户端不想或不能直接访问一个对象，此时可以通过一个称之为“代理”的第三者来实现间接访问，该方案对应的设计模式被称为代理模式。
```
代理模式下，为了保证客户端使用的透明性，所访问的**真实对象与代理对象需要实现相同的接口**。
```

#### 1. 案例： 代购
代购通常分为两种类型：
* 因为在当地买不到某件商品，又或者是因为当地这件商品的价格比其他地区的贵。 （适配器模式）
* 消费者对想要购买的商品相关信息的缺乏。 （外观模式）
```
代理三兄弟： 适配器、外观模式、代理模式
```

#### 2. 代理模式
<div align="center"><img src="pics/proxy-pattern-1.jpg" width="55%"></div>

##### 2.1 特征
```
▪ Subject （抽象主题角色）： 它声明了真实主题和代理主题的共同接口，这样一来在任何使用真实主题的地方
                         都可以使用代理主题，客户端通常需要针对抽象主题角色进行编程。
▪ 真实（具体）主题类、代理类都是抽象主题类的子类，以保证接口一致。 “依赖倒转原则”。  （装饰器模式）
```

##### 2.2 代码框架
```java
# 1. 抽象主题定义 （确定共同接口）

abstract class Subject
{
    public abstract void Request();
}

# 2. 真实主题

class RealSubject : Subject
{
    public override void Request()
    {
        //业务方法具体实现代码
    }
}

# 3. 代理 （代理类也是抽象主题类的子类！！）

class Proxy : Subject
{
    private RealSubject realSubject = new RealSubject(); //维持一个对真实主题对象的引用
 
    public void PreRequest() 
    {
        ……
    }
 
    public override void Request() 
    {
        PreRequest();
        realSubject.Request(); //调用真实主题对象的方法
        PostRequest();
    }
 
    public void PostRequest() 
    {
        ……
    }
}

# 4. 主程序 （main）

……
Proxy proxy = new Proxy();
proxy.Request();
……
```

##### 2.3 代理的种类
代理模式根据其目的和实现方式不同可分为很多种类。
```
▫ 远程代理 （Remote Proxy）   - 为一个位于不同的地址空间（其他域）的对象提供一个本地的代理对象。
▫ 虚拟代理 （Virtual Proxy）  - 如果需要创建一个资源消耗较大的对象，先创建一个消耗相对较小的对象来表示，
                              真实对象只在需要时才会被真正创建。  （Initialization on Demand Holder (IoDH)）
▫ 保护代理 （Protect Proxy）  - 一个对象的访问提供不同级别的权限。  （装饰器模式）
▫ 缓冲代理 （Cache Proxy）    - 为某一个目标操作的结果提供临时的存储空间，以便多个客户端可以共享这些结果。 （享元工厂）
▫ 智能引用代理 （Smart Reference Proxy）  - 当一个对象被引用时，提供一些额外的操作，例如将对象被调用的次数记录下来等。  （装饰器模式）
```

#### 3. 案例2： 商务信息查询系统
商务信息查询系统的基本需求： (1) 查询之前用户需要通过身份验证；（2） 查询时系统需要记录查询日志。
```
开发人员已完成了商务信息查询模块的开发任务。 以一种松耦合的方式向原有系统增加身份验证和日志记录功能，
客户端代码可以无区别地对待原始查询模块和新的查询模块。  (透明装饰器模式)
```
<div align="center"><img src="pics/proxy-pattern-2.jpg" width="65%"></div>

代码框架
```java
# 1. 身份验证业务类

// AccessValidator.cs
using System;

namespace ProxySample
{
    class AccessValidator
    {
        //模拟实现登录验证
        public bool Validate(string userId) 
        {
		    Console.WriteLine("在数据库中验证用户'" + userId + "'是否是合法用户？");
		    if (userId.Equals("杨过")) {
			    Console.WriteLine("'{0}'登录成功！",userId);
			    return true;
		    }
		    else {
			    Console.WriteLine("'{0}'登录失败！", userId);
			    return false;
		    }
        }
    }
}

# 2. 日志记录业务类

// Logger.cs
using System;
 
namespace ProxySample
{
    class Logger
    {
        //模拟实现日志记录
        public void Log(string userId) {
            Console.WriteLine("更新数据库，用户'{0}'查询次数加1！",userId);
        }
    }
}

# 3. 抽象查询类 （抽象主题类）

// Searcher.cs
namespace ProxySample
{
    interface Searcher
    {
        string DoSearch(string userId, string keyword);
    }
}

# 4. 具体查询类

// RealSearcher.cs
using System;
 
namespace ProxySample
{
    class RealSearcher : Searcher
    {
        //模拟查询商务信息
        public string DoSearch(string userId, string keyword) {
            Console.WriteLine("用户'{0}'使用关键词'{1}'查询商务信息！",userId,keyword);
            return "返回具体内容";
        }
    }
}

# 5. 代理类

// ProxySearcher.cs
namespace ProxySample
{
    class ProxySearcher : Searcher
    {
        private RealSearcher searcher = new RealSearcher(); //维持一个对真实主题的引用
        private AccessValidator validator;
        private Logger logger;
 
        public string DoSearch(string userId, string keyword)
        {
            //如果身份验证成功，则执行查询
            if (this.Validate(userId))
            {
                string result = searcher.DoSearch(userId, keyword); //调用真实主题对象的查询方法
                this.Log(userId); //记录查询日志
                return result; //返回查询结果
            }
            else
            {
                return null;
            }
        }
 
        //创建访问验证对象并调用其Validate()方法实现身份验证
        public bool Validate(string userId)
        {
            validator = new AccessValidator();
            return validator.Validate(userId);
        }
 
        //创建日志记录对象并调用其Log()方法实现日志记录
        public void Log(string userId)
        {
            logger = new Logger();
            logger.Log(userId);
        }
    }
}

# 6. 配置文件 App.config

<?xml version="1.0" encoding="utf-8" ?>
<configuration>
  <appSettings>
    <add key="proxy" value="ProxySample.ProxySearcher"/>
  </appSettings>
</configuration>

# 7. 主程序 （main）

// Program.cs
using System;
using System.Configuration;
using System.Reflection;
 
namespace ProxySample
{
    class Program
    {
        static void Main(string[] args)
        {
            //读取配置文件
            string proxy = ConfigurationManager.AppSettings["proxy"];
 
            //反射生成对象，针对抽象编程，客户端无须分辨真实主题类和代理类
            Searcher searcher;
            searcher = (Searcher)Assembly.Load("ProxySample").CreateInstance(proxy);
 
            String result = searcher.DoSearch("杨过", "玉女心经");
            Console.Read();
        }
    }
}
```
特别说明
```
本实例是**保护代理**和**智能引用代理**的应用实例，
在代理类 ProxySearcher 中实现对真实主题类的权限控制和引用计数。
```

#### 4. 其他代理模式

##### 4.1 远程代理
远程代理 (Remote Proxy) 是一种常用的代理模式，将网络的细节隐藏起来，使得客户端不必考虑网络的存在。 （远程代理对象承担了大部分的网络通信工作，并负责对远程业务方法的调用。） 分布式技术中都应用了远程代理模式。
<div align="center"><img src="pics/proxy-pattern-3.jpg" width="45%"></div>

##### 4.2 虚拟代理
虚拟代理 (Virtual Proxy) 也是一种常用的代理模式，对于一些占用系统资源较多或者加载时间较长的对象，在真实对象创建成功之前虚拟代理扮演真实对象的替身。

##### 4.3 缓冲代理
缓冲代理 (Cache Proxy) 也是一种较为常用的代理模式，它为某一个操作的结果提供临时的缓存存储空间，以便在后续使用中能够共享这些结果，可以避免某些方法的重复执行，优化系统性能。
<div align="center"><img src="pics/proxy-pattern-4.jpg" width="50%"></div>

```java
# Cache Proxy 缓冲机制

public static class ProductDataProxy
{
    private static readonly int productTimeout = int.Parse(ConfigurationManager.AppSettings ["ProductCacheDuration"]);
    private static readonly bool enableCaching = bool.Parse(ConfigurationManager. AppSettings["EnableCaching"]); 
 
    public static IList GetProductsByCategory(string category)
    {        
        Product product = new Product();
 
        //如果缓存被禁用，则直接通过product对象来获取数据
        if (!enableCaching)
        {
            return product.GetProductsByCategory(category);
        }
 
        string key = "product_by_category_" + category;
        //从缓存中获取数据
        IList data = (IList)HttpRuntime.Cache[key];  
 
        //如果缓存中没有数据则执行如下代码
        if (data == null)
        {            
            data = product.GetProductsByCategory(category);   
            //通过工厂创建AggregateCacheDependency对象
            AggregateCacheDependency cd = DependencyFacade.GetProductDependency (); 
            //将数据存储在缓存中，并添加必要的AggregateCacheDependency对象
            HttpRuntime.Cache.Add(key, data, cd, DateTime.Now.AddHours(product Timeout), Cache.NoSlidingExpiration, CacheItemPriority.High, null); 
        }
        return data;
    }
        ……
}
```

#### 5. 本模式优缺点
```
优：
▪ 一定程度上降低系统（客户端/业务）的耦合度。
▪ 针对抽象主题角色进行编程，符合开闭原则。
▪ 远程代理、虚拟代理、缓冲代理、保护代理 等具有各自的优点。
缺：
▪ 有些类型的代理模式可能会造成请求的处理速度变慢，例如保护代理。
```
适用场景
```
远程代理
虚拟代理
缓冲代理
保护代理
智能引用代理
（比较 代理模式 与 装饰器模式 的区别）
```
