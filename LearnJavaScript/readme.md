# JavaScript Learning Note
* For basic syntax, please read: [js_notes.md](js_notes.md)
* **This note contains some advance feature:**


## 1. => 函数
ES6 标准新增了一种新的函数：Arrow Function（箭头函数）

```javascript
x => x * x

此箭头函数相当于：
function (x) {
    return x * x;
}
```

```javascript
var fn = x => x * x;
```
箭头函数相当于匿名函数，并且简化了函数定义。

##### 箭头函数有两种格式:
1. 一种像上面的，只包含一个表达式，连 **\{ ... }** 和 **return** 都省略掉了。
1. 另一种可以包含多条语句，这时候就不能省略 \{ ... } 和 return。
```javascript
x => {
    if (x > 0) {
        return x * x;
    }
    else {
        return - x * x;
    }
}
```

##### 注意事项：
* 如果参数不是一个，就需要用括号 () 括起来：
```javascript
// 一个参数：
x => x * x

// 两个参数:
(x, y) => x * x + y * y

// 无参数:
() => 3.14
```
* 箭头函数看上去是匿名函数的一种简写，但实际上，箭头函数和匿名函数有个明显的区别：**箭头函数内部的 this 是词法作用域，由上下文确定**：
```javascript
由于 JavaScript 函数对 this 绑定的错误处理, 该例子无法得到预期结果：
var obj = {
    birth: 1990,
    getAge: function () {
        var b = this.birth; // 1990
        var fn = function () {
            return new Date().getFullYear() - this.birth; // this指向window或undefined
        };
        return fn();
    }
};
```
```javascript
箭头函数完全修复了 this 的指向，this 总是指向词法作用域，也就是外层调用者 obj:
var obj = {
    birth: 1990,
    getAge: function () {
        var b = this.birth; // 1990
        var fn = () => new Date().getFullYear() - this.birth; // this指向obj对象
        return fn();
    }
};
obj.getAge(); 
```
* 由于 this 在箭头函数中已经按照词法作用域绑定了，所以，用 call() 或者 apply() 调用箭头函数时，无法对 this 进行绑定，即传入的第一个参数被忽略:
```javascript
var obj = {
    birth: 1990,
    getAge: function (year) {
        var b = this.birth; // 1990
        var fn = (y) => y - this.birth; // this.birth仍是1990
        return fn.call({birth:2000}, year);
    }
};
obj.getAge(2015); // 25
```


## 2. JavaScript 对象
#### 构造方法 （Constructor）
* 方法一：**var**
```html
<html>
   <head>
      <title>User-defined objects</title>
      
      <script type="text/javascript">
         var book = new Object();   // Create the object
         book.subject = "Perl"; // Assign properties to the object
         book.author  = "Mohtashim";
      </script>
      
   </head>
   <body>
   
      <script type="text/javascript">
         document.write("Book name is : " + book.subject + "<br>");
         document.write("Book author is : " + book.author + "<br>");
      </script>
   
   </body>
</html>
```
* 方法二: **this**
```html
<html>
   <head>
   <title>User-defined objects</title>
   
      <script type="text/javascript">
         function book(title, author){
            this.title = title; 
            this.author  = author;
         }
      </script>
      
   </head>
   <body>
   
      <script type="text/javascript">
         var myBook = new book("Perl", "Mohtashim");
         document.write("Book title is : " + myBook.title + "<br>");
         document.write("Book author is : " + myBook.author + "<br>");
      </script>
      
   </body>
</html>
```

#### 成员函数定义 （Methods）
* 定义方式: **this**
```html
<html>
   <head>
   <title>User-defined objects</title>
   
      <script type="text/javascript">
      
         // define a function which will work as a method
         function addPrice(amount){
            this.price = amount; 
         }
         
         function book(title, author){
            this.title = title;
            this.author  = author;
            this.addPrice = addPrice; // assign that method as property. (将函数名赋给成员)
         }
         
      </script>
      
   </head>
   <body>
   
      <script type="text/javascript">
         var myBook = new book("Perl", "Mohtashim");
         myBook.addPrice(100);
         
         document.write("Book title is : " + myBook.title + "<br>");
         document.write("Book author is : " + myBook.author + "<br>");
         document.write("Book price is : " + myBook.price + "<br>");
      </script>
      
   </body>
</html>
```

#### 'with' 语法特性
被传递给 with 的对象，将成为后续局部块的默认对象，变量引用时优先被考虑为对象的成员变量：
```html
<html>
   <head>
   <title>User-defined objects</title>
   
      <script type="text/javascript">
      
         // define a function which will work as a method
         function addPrice(amount){
            with(this){
               price = amount; // this statement uses price instead of this.price （price 调用等价于 this.price, 其中 this 这个对象在书写中被省略）
            }
         }
         
         function book(title, author){
            this.title = title;
            this.author  = author;
            this.price = 0;
            this.addPrice = addPrice; // assign that method as property. (将函数名赋给成员)
         }
         
      </script>
      
   </head>
   <body>
   
      <script type="text/javascript">
         var myBook = new book("Perl", "Mohtashim");
         myBook.addPrice(100);
         
         document.write("Book title is : " + myBook.title + "<br>");
         document.write("Book author is : " + myBook.author + "<br>");
         document.write("Book price is : " + myBook.price + "<br>");
      </script>
      
   </body>
</html>
```


## 3. js中2个等号与3个等号的区别
###### ==， 两边值类型相同时，等同于===；不同的时候，要先进行类型转换，再比较。 <br>
###### ===，不做类型转换，类型不同的一定不等。



## 4. javascript 中的 require、import 和 export.

###### Javascript 不是一种模块化编程语言，在 ES6 以前，它是不支持”类”（class），所以也就没有”模块”（module）。

#### Step 1: 蛮荒 时代
##### Javascript社区做了很多努力，在现有的运行环境中，实现”模块”的效果。

* 原始写法
```html
function m1(){
　//...
}
function m2(){
　//...　　
}
# ”污染”了全局变量，
# 无法保证不与其他模块发生变量名冲突
# 而且模块成员之间看不出直接关系。
```

* 对象写法
```html
var module1 = new Object({
_count : 0,
　m1 : function (){
　　//...
　},
　m2 : function (){
　　//...
　}
});
# 暴露所有模块成员，内部状态可以被外部改写
```

* 立即执行函数写法
```html
var module = (function() {
var _count = 0;
var m1 = function() {
alert(_count)
}
var m2 = function() {
alert(_count + 1)
}
 
return {
m1: m1,
m2: m2
}
})()
# module 就是 Javascript 模块的基本写法
```

#### Step 2: 主流模块规范 (module )
###### 在 ES6 以前，还没有提出一套官方的规范，从社区和框架推广程度而言，目前通行的 javascript 模块规范有两种：CommonJS 和 AMD。

2009 年，美国程序员 Ryan Dahl 创造了 node.js 项目，将 javascript 语言用于服务器端编程。这标志 ”Javascript模块化编程” 正式诞生。node 编程中最重要的思想之一就是模块。

* CommonJS 规范
```html
# 在 CommonJS 中,暴露模块使用 module.exports 和 export。
# 在 CommonJS 中，有一个全局性方法 require()，用于加载模块。

var math = require('math');
math.add(2,3); // 5
```

* AMD 规范
###### AMD是”Asynchronous Module Definition”的缩写，意思就是”异步模块定义”。它采用异步方式加载模块，模块的加载不影响它后面语句的运行。所有依赖这个模块的语句，都定义在一个回调函数中，等到加载完成之后，这个回调函数才会运行。
```html
有了服务器端模块以后，很自然地，大家就想要客户端模块。
由于一个重大的局限，使得CommonJS规范不适用于浏览器环境。

var math = require('math');  # math.js 加载
math.add(2, 3);

如果加载时间很长，整个应用就会停在那里等。
```
```html
模块必须采用特定的 define() 函数来定义
define(id?, dependencies?, factory)
▪ id:字符串，模块名称(可选)
▪ dependencies: 是我们要载入的依赖模块(可选)，使用相对路径。注意是数组格式
▪ factory: 工厂方法，返回一个模块函数

AMD 也采用 require() 语句加载模块，但是不同于 CommonJS，它要求两个参数：
require([module], callback);

第一个参数 [module]，是一个数组，里面的成员就是要加载的模块；第二个参数 callback，则是加载成功之后执行的回调函数。
```

###### 目前，主要有两个 Javascript 库实现了 AMD 规范：require.js 和 curl.js。

* CMD 规范

###### CMD (Common Module Definition), 是 seajs 推崇的规范，CMD 则是依赖就近，用的时候再 require。

```html
CMD 与 AMD 一样，也是采用特定的 define() 函数来定义,用 require 方式来引用模块。
```

##### CMD 与 AMD 的区别
AMD 和 CMD 最大的区别是对依赖模块的执行时机处理不同，而不是加载的时机或者方式不同，二者皆为异步加载模块。
* AMD 依赖前置，js 可以方便知道依赖模块是谁，立即加载；
* CMD 就近依赖，需要使用把模块变为字符串解析一遍才知道依赖了那些模块，这也是很多人诟病 CMD 的一点。

***CMD 牺牲性能以带来开发的便利性，实际上解析模块用的时间短到可以忽略。***

#### Step 3： 现阶段的标准 (ES6 标准)

###### ES6 标准发布后，module 成为标准，标准使用是以 export 指令导出接口，以 import 引入模块，但是在我们一贯的 node 模块中，我们依然采用的是 CommonJS 规范，使用 require 引入模块，使用 module.exports 导出接口。

* export 导出模块

export 有两种模块导出方式：**命名式导出**（名称导出）和**默认导出**（定义式导出），命名式导出每个模块可以多个，而默认导出每个模块仅一个。
###### 默认导出可以理解为另一种形式的命名导出，默认导出可以认为是使用了default名称的命名导出

* import 引入模块

import 模块导入与 export 模块导出功能相对应，也存在两种模块导入方式：**命名式导入**（名称导入）和**默认导入**（定义式导入）。
###### import 的语法跟 require 不同，而且 import 必须放在文件的最开始，且前面不允许有其他逻辑代码，这和其他所有编程语言风格一致。

```html
import a from './d';
// 等效于，或者说就是下面这种写法的简写，是同一个意思
import {default as a} from './d';

简单的说，如果 import 的时候，你发现某个变量没有花括号括起来（没有*号），那么你在脑海中应该把它还原成有花括号的 as 语法。
```



###### reference: https://www.cnblogs.com/libin-1/p/7127481.html
