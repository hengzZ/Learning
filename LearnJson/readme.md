## JSON
***JSON or JavaScript Object Notation is a lightweight text-based open standard designed for human-readable data interchange.*** <br>
JSON Internet Media type is **application/json**.

###### JSON 文件大多以 { 开头。一个文件即是一个对象，Array 也是 javascript 对象。
###### javascript 是一个面向对象语言，对象是最基本的数据类型（primitive）。


#### JSON - Syntax

First, Let's see a piece of code **\[json.html]**:
```html
<html>
   <head>
      <title>JSON example</title>
      <script language = "javascript" >
         var object1 = { "language" : "Java", "author"  : "herbert schildt" };
         document.write("<h1>JSON with JavaScript example</h1>");
         document.write("<br>");
         document.write("<h3>Language = " + object1.language+"</h3>");  
         document.write("<h3>Author = " + object1.author+"</h3>");   

         var object2 = { "language" : "C++", "author"  : "E-Balagurusamy" };
         document.write("<br>");
         document.write("<h3>Language = " + object2.language+"</h3>");  
         document.write("<h3>Author = " + object2.author+"</h3>");   
  
         document.write("<hr />");
         document.write(object2.language + " programming language can be studied " + "from book written by " + object2.author);
         document.write("<hr />");
      </script>
   </head>
   
   <body>
   </body>
</html>
```

***JSON syntax is basically considered as a subset of JavaScript syntax;*** it includes the following −
1. 数据以键值对表示。(Data is represented in name/value pairs.)
1. 大括号声明一个对象，每个名字后面跟随一个冒号（:），键值对以逗号（,）分隔。
1. 方括号声明一个数组，每个值以逗号（,）分隔。

示例：
```
{
   "book": [

      {
         "id": "01",
         "language": "Java",
         "edition": "third",
         "author": "Herbert Schildt"
      },

      {
         "id": "07",
         "language": "C++",
         "edition": "second",
         "author": "E.Balagurusamy"
      }

   ]
}
```

***JSON supports the following two data structures*** −
* **Collection of name/value pairs** − This Data Structure is supported by different programming languages.
* **Ordered list of values** − It includes array, list, vector or sequence etc.


#### JSON - DataTypes

json 格式支持的数据类型：
* Number - double- precision floating-point format in JavaScript
    ```
    1. 统一为双精度浮点类型
    2. 不使用八进制和十六进制格式
    3. 没有 NaN 或 Infinity
    ```
* String - double-quoted Unicode with backslash escaping
    ```
    不区分字符与字符串类型
    ```
* Boolean - true or false
    ```
    true
    false
    ```    
* null - empty
    ```
    null
    ```
* Array - an ordered sequence of values
    ```
    [ value, .......]
    ```
* Object - an unordered collection of key:value pairs
    ```
    { string : value, .......}
    ```

##### ordered v.s. unordered
***ordered collection of values*** <br>
***unordered set of name/value pairs*** <br>
```
ordered 数据一定为 Array，且使用 idx 检索。
```

##### Array v.s. Object
***Arrays should be used when the key names are sequential integers.*** <br>
***Objects should be used when the key names are arbitrary strings.*** <br>
```
1. KEY 理念 （idx or key） - idx 与 key 都为 KEY NAME
2. 检索方式的不同： idx 检索 or 键值检索
```


#### JSON - Objects

对象的创建方式：
```javascript
// 创建一个空对象 
var JSONObj = {};

// 创建一个新对象
var JSONObj = new Object();
```

***对象内的每一个键值对叫做 attribute (属性)， 使用点（.）操作符访问属性的值。***
 

#### JSON - Comparison with XML
*JSON and XML are human readable formats and are language independent.*

***Verbose （啰嗦/冗长）*** <br>
XML is more verbose than JSON, so it is faster to write JSON for programmers.

***Arrays Usage*** <br>
XML is used to describe the structured data, which doesn't include arrays whereas JSON include arrays.

***Parsing*** <br>
JavaScript's eval method parses JSON. When applied to JSON, eval returns the described object.

<br>

```
JSON
{
   "company": Volkswagen,
   "name": "Vento",
   "price": 800000
}

XML
<car>
   <company>Volkswagen</company>
   <name>Vento</name>
   <price>800000</price>
</car>
```


#### JSON with Python
***How to encode and decode JSON objects using Python programming language.***

```
# Demjson 工具
$tar xvfz demjson-1.6.tar.gz
$cd demjson-1.6
$python setup.py install
```

Functions:
```
encode - Encodes the Python object into a JSON string representation.
decode - Decodes a JSON-encoded string into a Python object.
```

示例：
```python
#!/usr/bin/python
import demjson

data = [ { 'a' : 1, 'b' : 2, 'c' : 3, 'd' : 4, 'e' : 5 } ]

json = demjson.encode(data)
print json

[输出]
[{"a":1,"b":2,"c":3,"d":4,"e":5}]

___________________________________________________________
#!/usr/bin/python
import demjson

json = '{"a":1,"b":2,"c":3,"d":4,"e":5}';

text = demjson.decode(json)
print  text

[输出]
{u'a': 1, u'c': 3, u'b': 2, u'e': 5, u'd': 4}
```


###### reference
[1] https://www.tutorialspoint.com/json/index.htm
