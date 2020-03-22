# JavaScript Learning Note
* 基本语法了解，请查看 basic-concepts.md 文件，或者 https://github.com/hengzZ/peter/tree/master/h5-css-javascript 的内容。
* 本文档主要记录一些 高级特性的讲解。


## 1. => 函数 （箭头函数）

> ES6 标准新增了一种新的函数：Arrow Function（箭头函数）

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

##### 1.1 箭头函数的两种格式
* 一种像上面的，只包含一个表达式，连 **\{ ... }** 和 **return** 都省略掉了。
* 另一种可以包含多条语句，这时候就不能省略 \{ ... } 和 return。
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

##### 1.2 注意事项
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

### 总结

#### 1.基础语法
```javascript
(param1, param2, …, paramN) => { statements }
(param1, param2, …, paramN) => expression
//相当于：(param1, param2, …, paramN) =>{ return expression; }

// 当只有一个参数时，圆括号是可选的：
(singleParam) => { statements }
singleParam => { statements }

// 没有参数的函数应该写成一对圆括号。
() => { statements }
```

#### 2.高级语法
```javascript
//加括号的函数体返回对象字面量表达式：
params => ({foo: bar})

//支持剩余参数和默认参数
(param1, param2, ...rest) => { statements }
(param1 = defaultValue1, param2, …, paramN = defaultValueN) => { statements }

//同样支持参数列表解构
let f = ([a, b] = [1, 2], {x: c} = {x: a + b}) => a + b + c;
f();  // 6
```

#### 3.为什么会有引入箭头函数的想法

> 引入箭头函数有两个方面的作用：更简短的函数并且不绑定 this。

* 在箭头函数出现之前，每一个新函数根据它是被如何调用的来定义这个函数的 this 值！！！ 即，
   - 如果是该函数是一个构造函数，this 指针指向一个新的对象。
   - 在严格模式下的函数调用下，this 指向 undefined。
   - 如果是该函数是一个对象的方法，则它的 this 指针指向这个对象。
   - 等等

> ``this`` 被证明是令人厌烦的面向对象风格的编程。
> 在 ECMAScript 3/5 中，通过将 this 值分配给封闭的变量，以解决 this 问题。 ``let vm = this;``

> 而箭头函数不会创建自己的 this，它只会从自己的作用域链的上一层继承 this。

**Mozilla 基金权威 JavaScript 教程：** https://developer.mozilla.org/zh-CN/docs/Web/JavaScript 。

<br>

## 2. JavaScript 对象
#### 2.1 构造方法 / 构造函数 （Constructor）
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

#### 2.2 成员方法 / 成员函数 （Methods）
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

#### 2.3 'with' 语法特性
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

<br>

## 3. JavaScript 中 2 个等号与 3 个等号的区别
###### ==， 两边值类型相同时，等同于===；不同的时候，要先进行类型转换，再比较。 <br>
###### ===，不做类型转换，类型不同的一定不等。

<br>

## 4. JavaScript 中的 require、import 和 export.

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

<br>

## 5. JavaScript 中的 .then() 语法

JavaScript Promise 实例 then() 方法
* 描述 <br>
  then() 方法用于指定当前实例状态发生改变时的回调函数。 它返回一个新的 Promise 实例。
* 语法 <br>
```html
  Promise.prototype.then(onFulfilled, onRejected);
  <!-- onFulfilled 参数， 当前实例变成 fulfilled 状态时，该参数作为回调函数被调用。 -->
  <!-- onRejected 参数， 当前实例变成 reject 状态时，该参数作为回调函数被调用。 -->
```

##### 5.1 什么是 Promise？
Promise 是抽象异步处理对象以及对其进行各种操作的组件。 Promise 对象 预设了抽象化异步处理 机制/接口。
```html
下面是使用了Promise进行异步处理的一个例子
----
var promise = getAsyncPromise("fileA.txt");  // <1> 
promise.then(function(result){
    // 获取文件内容成功时的处理
}).catch(function(error){
    // 获取文件内容失败时的处理
});
----
<1> 返回 promise 对象
```

##### 5.2 Promise 构造方法 / 构造函数
```html
var promise = new Promise(function(resolve, reject) {
    // 异步处理
    // 处理结束后、调用resolve 或 reject
});
```

##### 5.3 成员方法 / 成员函数
then() 是为了 设置 Promise 对象其值在 resolve(成功) / reject(失败) 时调用的回调函数。

注意：
* 可以只采用 then() 方法，也可以采用 then() + catch() 的方法。

##### 5.4 Promise 对象还拥有一些静态方法
包括 Promise.all() 还有 Promise.resolve() 等在内，主要都是一些对 Promise 进行操作的辅助方法。

##### 5.5 一个简单实例
```html
function asyncFunction() {
    
    return new Promise(function (resolve, reject) {
        setTimeout(function () {
            resolve('Async Hello world');
        }, 16);
    });
}

asyncFunction().then(function (value) {
    console.log(value);    // => 'Async Hello world'
}).catch(function (error) {
    console.log(error);
});
```

###### reference
* http://liubin.org/promises-book/
* https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise

<br>

# Promise

> Promise 对象用于表示一个异步操作的最终完成 (或失败)，及其结果值。

因为大多数人仅仅是使用已创建的 Promise 实例对象，所以本教程将首先说明怎样使用 Promise，再说明如何创建 Promise。

—— https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Promise

<br>

## 1.使用Promise

本质上Promise是一个函数返回的对象，我们可以在它上面绑定回调函数，这样就不需要在一开始把回调函数作为参数传入这个返回Promise对象的函数了。

> 现在，假设有一个名为 `createAudioFileAsync()` 的函数，它接收一些配置和两个回调函数，然后异步地生成音频文件。一个回调函数在文件成功创建时的被调用，另一个则在出现异常时的被调用。

以下为使用 `createAudioFileAsync()` 的示例：

```javascript
// 成功的回调函数
function successCallback(result) {
  console.log("音频文件创建成功: " + result);
}

// 失败的回调函数
function failureCallback(error) {
  console.log("音频文件创建失败: " + error);
}

createAudioFileAsync(audioSettings, successCallback, failureCallback)
```



> 更现代的函数会返回一个 promise 对象，使得你可以将你的回调函数绑定在该 promise 上。

如果函数 `createAudioFileAsync()` 被重写为返回promise的形式，我们就可以像下面这样简单地调用它：

```javascript
const promise = createAudioFileAsync(audioSettings); 
promise.then(successCallback, failureCallback);
```

简写形式为

```javascript
createAudioFileAsync(audioSettings).then(successCallback, failureCallback);
```



我们把这个称为*异步函数调用*，这种形式有若干优点：

**不同于“老式”的传入回调，在使用 Promise 时，会有以下约定：**

- 在本轮 [事件循环](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/EventLoop#执行至完成) 运行完成之前，回调函数是不会被调用的。
- 即使异步操作已经完成（成功或失败），在这之后通过 [`then()` ](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise/then)添加的回调函数也会被调用。
- 通过多次调用 `then()` 可以添加多个回调函数，它们会按照插入顺序执行。

Promise 很棒的一点就是**链式调用**（**chaining**）。



#### 链式调用

连续执行两个或者多个异步操作是一个常见的需求，在上一个操作执行成功之后，开始下一个的操作，并带着上一步操作所返回的结果。

> Promise 出现的原因是：通过创造一个 **Promise 链**来实现这种需求。

见证奇迹的时刻：`then()` 函数会返回一个和原来不同的**新的Promise**：

```javascript
const promise2 = doSomething().then(successCallback, failureCallback);
```

> promise2 不仅表示 doSomething() 函数的完成，也代表了你传入的 successCallback 或者 failureCallback 的完成，这两个函数也可以返回一个 Promise 对象，从而形成另一个异步操作，这样的话，在 promise2 上新增的回调函数会排在这个 Promise 对象的后面。



现在，我们可以开心的把回调绑定到返回的 Promise 上，形成一个 Promise 链：（语法糖）

```javascript
doSomething().then(function(result) {
  return doSomethingElse(result);
})
.then(function(newResult) {
  return doThirdThing(newResult);
})
.then(function(finalResult) {
  console.log('Got the final result: ' + finalResult);
})
.catch(failureCallback);
```

then里的参数是可选的，`catch(failureCallback)` 是 `then(null, failureCallback)` 的缩略形式。



如下所示，我们也可以用箭头函数来表示：

```javascript
doSomething()
.then(result => doSomethingElse(result))
.then(newResult => doThirdThing(newResult))
.then(finalResult => {
  console.log(`Got the final result: ${finalResult}`);
})
.catch(failureCallback);
```

**注意：一定要有返回值，否则，callback 将无法获取上一个 Promise 的结果。**



#### Catch 的后续链式操作

有可能会在一个回调失败之后继续使用链式操作，即 使用一个 `catch`，这对于在链式操作中抛出一个失败之后，再次进行新的操作很有用。

```javascript
new Promise((resolve, reject) => {
    console.log('初始化');

    resolve();
})
.then(() => {
    throw new Error('有哪里不对了');
        
    console.log('执行「这个」”');
})
.catch(() => {
    console.log('执行「那个」');
})
.then(() => {
    console.log('执行「这个」，无论前面发生了什么');
});
```



#### 错误传递

以下示例有 3 次 failureCallback 的调用，而在 Promise 链中只有尾部的一次调用。

```javascript
doSomething()
.then(result => doSomethingElse(value))
.then(newResult => doThirdThing(newResult))
.then(finalResult => console.log(`Got the final result: ${finalResult}`))
.catch(failureCallback);
```

通常，一遇到异常抛出，浏览器就会顺着promise链寻找下一个 `onRejected` 失败回调函数或者由 `.catch()` 指定的回调函数。

这和以下的同步代码的执行过程很相似：

```javascript
try {
  let result = syncDoSomething();
  let newResult = syncDoSomethingElse(result);
  let finalResult = syncDoThirdThing(newResult);
  console.log(`Got the final result: ${finalResult}`);
} catch(error) {
  failureCallback(error);
}
```

> 在 ECMAScript 2017 标准的 `async/await` 语法糖中，这种与同步形式代码的对称性得到了极致的体现：

```javascript
async function foo() {
  try {
    const result = await doSomething();
    const newResult = await doSomethingElse(result);
    const finalResult = await doThirdThing(newResult);
    console.log(`Got the final result: ${finalResult}`);
  } catch(error) {
    failureCallback(error);
  }
}
```

这个例子是在 Promise 的基础上构建的，例如，`doSomething()` 与之前的函数是相同的。你可以在[这里](https://developers.google.com/web/fundamentals/getting-started/primers/async-functions)阅读更多的与此语法相关的文章。



至此，Promise 解决了回调地狱的基本缺陷。这对于构建异步操作的基础功能而言是很有必要的。



#### Promise 拒绝事件

>  当 Promise 被拒绝时，会有下文所述的两个事件之一被派发到全局作用域（通常而言，就是[`window`](https://developer.mozilla.org/zh-CN/docs/Web/API/Window)；如果是在 web worker 中使用的话，就是 [`Worker`](https://developer.mozilla.org/zh-CN/docs/Web/API/Worker) 或者其他 worker-based 接口）。

这两个事件如下所示：

- [`rejectionhandled`](https://developer.mozilla.org/zh-CN/docs/Web/API/Window/rejectionhandled_event)

  当 Promise 被拒绝、并且在 `reject` 函数处理该 rejection 之后会派发此事件。

- **unhandledrejection**

  当 Promise 被拒绝，但没有提供 `reject` 函数来处理该 rejection 时，会派发此事件。

**以上两种情况中，PromiseRejectionEvent 事件都有两个属性，一个是 promise 属性，该属性指向被驳回的 Promise，另一个是 reason 属性，该属性用来说明 Promise 被驳回的原因。**



更多 Promise 的使用语法，以及 JavaScript 特性：请阅读 https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Guide/Using_promises 。

<br>


## 2.Promise的构造函数

```javascript
const promise1 = new Promise(function(resolve, reject) {
  /* executor 部分 */
  setTimeout(function() {
    resolve('foo');
  }, 300);
});

promise1.then(function(value) {
  console.log(value);
  // expected output: "foo"
});

console.log(promise1);
// expected output: [object Promise]
```

Promise 构造函数接收带有 `resolve` 和 `reject` 两个参数的函数 。**Promise构造函数执行时，立即执行传入函数的函数体内容，函数体的执行是在Promise构造函数返回所建promise实例对象前被调用。**



> `resolve` 和 `reject` 函数被调用时，会分别将 promise 的状态改为 *fulfilled（*完成）或 rejected（失败）。
>
> executor 部分通常会执行一些异步操作，一旦异步操作执行完毕(可能成功/失败)，要么调用 ``resolve`` 函数来将 promise 状态改成 *fulfilled*，要么调用 `reject`  函数将 promise 的状态改为 rejected。



如果在 executor 部分中抛出一个错误，那么该 promise 状态为 rejected。executor 部分的返回值被忽略。

*更多内容同上，请查阅权威文档。*