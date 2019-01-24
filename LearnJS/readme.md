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
