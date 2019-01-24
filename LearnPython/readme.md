# Python Quick Start

# Part I: Basic Syntax
## Python - Variable Types

Python has five standard data types −
* Numbers
    - int (signed integers)
    - long (long integers, they can also be represented in octal and hexadecimal)
    - float (floating point real values)
    - complex (complex numbers)
* String
* List
* Tuple
* Dictionary

**Data Type Conversion** - 
1. int(x [,base])
1. long(x [,base] )
1. float(x)
1. complex(real [,imag])
1. str(x)
1. repr(x)
1. eval(str)
1. tuple(s)	
1. list(s)
1. set(s)
1. dict(d)
1. frozenset(s)
1. chr(x)
1. unichr(x)
1. ord(x)
1. hex(x)
1. oct(x)


## Python - Functions

### Pass by reference vs value

All parameters (arguments) in the Python language are passed by reference. 
```python
#!/usr/bin/python

# Function definition is here
def changeme( mylist ):
   "This changes a passed list into this function"
   mylist.append([1,2,3,4]);
   print "Values inside the function: ", mylist
   return

# Now you can call changeme function
mylist = [10,20,30];
changeme( mylist );
print "Values outside the function: ", mylist
```

Example: The parameters wasn't changed due to the reference is being overwritten inside the called function
```python
#!/usr/bin/python

# Function definition is here
def changeme( mylist ):
   "This changes a passed list into this function"
   mylist = [1,2,3,4]; # This would assig new reference in mylist
   print "Values inside the function: ", mylist
   return

# Now you can call changeme function
mylist = [10,20,30];
changeme( mylist );
print "Values outside the function: ", mylist
```

### Function Arguments

Default arguments
```python
#!/usr/bin/python

# Function definition is here
def printinfo( name, age = 35 ):
   "This prints a passed info into this function"
   print "Name: ", name
   print "Age ", age
   return;

# Now you can call printinfo function
printinfo( age=50, name="miki" )
printinfo( name="miki" )
```

Variable-length arguments
```python
#!/usr/bin/python

# Function definition is here
def printinfo( arg1, *vartuple ):
   "This prints a variable passed arguments"
   print "Output is: "
   print arg1
   for var in vartuple:
      print var
   return;

# Now you can call printinfo function
printinfo( 10 )
printinfo( 70, 60, 50 )
```

### The *Anonymous* Functions

Syntax:
lambda [arg1 [,arg2,.....argn]]:expression
```python
#!/usr/bin/python

# Function definition is here
sum = lambda arg1, arg2: arg1 + arg2;

# Now you can call sum as a function
print "Value of total : ", sum( 10, 20 )
print "Value of total : ", sum( 20, 20 )
```


## Python - Modules

### The dir( ) Function
The dir() built-in function returns a sorted list of strings containing the names defined by a module.

The list contains the names of all the modules, variables and functions that are defined in a module. Following is a simple example −
```python
#!/usr/bin/python

# Import built-in module math
import math

content = dir(math)
print content

[output]:
['__doc__', '__file__', '__name__', 'acos', 'asin', 'atan', 
'atan2', 'ceil', 'cos', 'cosh', 'degrees', 'e', 'exp', 
'fabs', 'floor', 'fmod', 'frexp', 'hypot', 'ldexp', 'log',
'log10', 'modf', 'pi', 'pow', 'radians', 'sin', 'sinh', 
'sqrt', 'tan', 'tanh']
```

### Packages in Python
* Phone/Isdn.py file having function Isdn()
* Phone/G3.py file having function G3()
* **Phone/__init__.py**

To make all of your functions available when you've imported **Phone**, you need to put explicit import statements in \_\_init__.py as follows −
```python
/* File name : __init__.py */
from Pots import Pots
from Isdn import Isdn
from G3 import G3
```

```python
/* File name : test.py */
#!/usr/bin/python

# Now import your Phone Package.
import Phone

Phone.Pots()
Phone.Isdn()
Phone.G3()
```


## Python - Exceptions Handling
```python
#!/usr/bin/python

try:
   fh = open("testfile", "w")
   try:
      fh.write("This is my test file for exception handling!!")
   finally:
      print "Going to close the file"
      fh.close()
except IOError:
   print "Error: can\'t find file or read data"
```


## Python - Object Oriented

example of a simple Python class −
```python
class Employee:
   'Common base class for all employees'
   empCount = 0

   def __init__(self, name, salary):
      self.name = name
      self.salary = salary
      Employee.empCount += 1
   
   def displayCount(self):
     print "Total Employee %d" % Employee.empCount

   def displayEmployee(self):
      print "Name : ", self.name,  ", Salary: ", self.salary
```

### Destroying Objects (Garbage Collection)
```python
#!/usr/bin/python

class Point:
   def __init__( self, x=0, y=0):
      self.x = x
      self.y = y
   def __del__(self):
      class_name = self.__class__.__name__
      print class_name, "destroyed"

pt1 = Point()
pt2 = pt1
pt3 = pt1
print id(pt1), id(pt2), id(pt3) # prints the ids of the obejcts
del pt1
del pt2
del pt3
```

### Class Inheritance
```python
#!/usr/bin/python

class Parent:        # define parent class
   parentAttr = 100
   def __init__(self):
      print "Calling parent constructor"

   def parentMethod(self):
      print 'Calling parent method'

   def setAttr(self, attr):
      Parent.parentAttr = attr

   def getAttr(self):
      print "Parent attribute :", Parent.parentAttr

class Child(Parent): # define child class
   def __init__(self):
      print "Calling child constructor"

   def childMethod(self):
      print 'Calling child method'

c = Child()          # instance of child
c.childMethod()      # child calls its method
c.parentMethod()     # calls parent's method
c.setAttr(200)       # again call parent's method
c.getAttr()          # again call parent's method
```


## Python - Regular Expressions

The match Function
```python
#!/usr/bin/python
import re

line = "Cats are smarter than dogs"

matchObj = re.match( r'(.*) are (.*?) .*', line, re.M|re.I)

if matchObj:
   print "matchObj.group() : ", matchObj.group()
   print "matchObj.group(1) : ", matchObj.group(1)
   print "matchObj.group(2) : ", matchObj.group(2)
else:
   print "No match!!"
```

The search Function<br>
This function searches for first occurrence of RE pattern within string with optional flags.
```python
#!/usr/bin/python
import re

line = "Cats are smarter than dogs";

searchObj = re.search( r'(.*) are (.*?) .*', line, re.M|re.I)

if searchObj:
   print "searchObj.group() : ", searchObj.group()
   print "searchObj.group(1) : ", searchObj.group(1)
   print "searchObj.group(2) : ", searchObj.group(2)
else:
   print "Nothing found!!"
```

### Matching Versus Searching
Python offers two different primitive operations based on regular expressions: **match** checks for a match only at the beginning of the string, while **search** checks for a match anywhere in the string (this is what Perl does by default).
```python
#!/usr/bin/python
import re

line = "Cats are smarter than dogs";

matchObj = re.match( r'dogs', line, re.M|re.I)
if matchObj:
   print "match --> matchObj.group() : ", matchObj.group()
else:
   print "No match!!"

searchObj = re.search( r'dogs', line, re.M|re.I)
if searchObj:
   print "search --> searchObj.group() : ", searchObj.group()
else:
   print "Nothing found!!"


[output]:
No match!!
search --> matchObj.group() :  dogs
```


# Part II: 【Pyhton Decorator】
【装饰器作用】<br>
python装饰器就是用于拓展原来函数功能的一种函数，这个函数的特殊之处在于它的返回值也是一个函数，使用python装饰器的好处就是在不用更改原函数的代码前提下给函数增加新的功能。

## 【装饰器演化过程】<br>
【1】避免直接侵入原函数修改，但是生效需要再次执行函数
```python
import time

def deco(func):
    startTime = time.time()
    func()
    endTime = time.time()
    msecs = (endTime - startTime)*1000
    print("time is %d ms" %msecs)


def func():
    print("hello")
    time.sleep(1)
    print("world")

if __name__ == '__main__':
    f = func
    deco(f) #只有把func()或者f()作为参数执行，新加入功能才会生效
    print("f.__name__ is",f.__name__) #f的name就是func()
    print()
```
【2】既不需要侵入，也不需要函数重复执行
```python
import time

def deco(func):
    def wrapper():
        startTime = time.time()
        func()
        endTime = time.time()
        msecs = (endTime - startTime)*1000
        print("time is %d ms" %msecs)
    return wrapper


@deco
def func():
    print("hello")
    time.sleep(1)
    print("world")

if __name__ == '__main__':
    f = func #这里f被赋值为func，执行f()就是执行func()
    f()
```
【3】带有参数的装饰器
```python
import time

def deco(func):
    def wrapper(a,b):
        startTime = time.time()
        func(a,b)
        endTime = time.time()
        msecs = (endTime - startTime)*1000
        print("time is %d ms" %msecs)
    return wrapper


@deco
def func(a,b):
    print("hello，here is a func for add :")
    time.sleep(1)
    print("result is %d" %(a+b))

if __name__ == '__main__':
    f = func
    f(3,4)
```
【4】带有不定参数的装饰器
```python
import time

def deco(func):
    def wrapper(*args, **kwargs):
        startTime = time.time()
        func(*args, **kwargs)
        endTime = time.time()
        msecs = (endTime - startTime)*1000
        print("time is %d ms" %msecs)
    return wrapper


@deco
def func(a,b):
    print("hello，here is a func for add :")
    time.sleep(1)
    print("result is %d" %(a+b))

@deco
def func2(a,b,c):
    print("hello，here is a func for add :")
    time.sleep(1)
    print("result is %d" %(a+b+c))


if __name__ == '__main__':
    f = func
    func2(3,4,5)
    f(3,4)
```
【5】多个装饰器
```python
import time

def deco01(func):
    def wrapper(*args, **kwargs):
        print("this is deco01")
        startTime = time.time()
        func(*args, **kwargs)
        endTime = time.time()
        msecs = (endTime - startTime)*1000
        print("time is %d ms" %msecs)
        print("deco01 end here")
    return wrapper

def deco02(func):
    def wrapper(*args, **kwargs):
        print("this is deco02")
        func(*args, **kwargs)
                 print("deco02 end here")
    return wrapper

@deco01
@deco02
def func(a,b):
    print("hello，here is a func for add :")
    time.sleep(1)
    print("result is %d" %(a+b))


if __name__ == '__main__':
    f = func
    f(3,4)
```

## 【Python 可变参数列表】<br>
当我们声明一个诸如 *param 的星号参数时，从此处开始直到结束的所有位置参数 （Positional Arguments）都将被收集并汇集成一个称为“param”的元组（Tuple）。 类似地，当我们声明一个诸如 **param 的双星号参数时，从此处开始直至结束的所有关键字 参数都将被收集并汇集成一个名为 param 的字典（Dictionary）。

【1】元组 字典
```python
def total(a=5, *numbers, **phonebook):
    print('a', a)
    #遍历元组中的所有项目
    for single_item in numbers:
        print('single_item', single_item)
    #遍历字典中的所有项目
    for first_part, second_part in phonebook.items():
        print(first_part,second_part)


if __name__ == "__main__":
    print(total(10,1,2,3,Jack=1123,John=2231,Inge=1560))
```

## 【Flask @app.route("/")实现】
【装饰器】<br>
要想明白“@app.route()”的工作原理，我们首先需要看一看Python中的装饰器（就是以“@”开头的那玩意，下面接着函数定义）。
当你装饰一个函数，意味着你告诉Python调用的是那个由装饰器返回的新函数，而不仅仅是直接返回原函数体的执行结果。
```python
def simple_decorator(f):
    # This is the new function we're going to return
    # This function will be used in place of our original definition
    def wrapper():
        print "Entering Function"
        f()
        print "Exited Function"
    return wrapper

@simple_decorator
def hello():
    print "Hello World"

hello()
```

【把“app”放进“app.route”】<br>
现在我们掌握了装饰器怎样工作的全部前置知识 ，可以重新实现Flask API的这个部分了，那么把我们的目光转移到“app”在我们Flask应用中的重要地位上面来。

1.在开始解释Flask对象里面发生了什么之前，我们先创建我们自己的Python类NotFlask。
```python
class NotFlask():
    pass

app = NotFlask()
```
2.类的方法也可以被用作装饰器，所以让我们把这个类写得更有趣一点，加一个称作 route的方法，它是一个简单的装饰器工厂。
```python
class NotFlask():
    def route(self, route_str):
        def decorator(f):
            return f
        return decorator

app = NotFlask()

@app.route("/")
def hello():
    return "Hello World!"
```
这个装饰器和我们之前创建的那个最大的不同，在于我们不想修改被我们装饰的函数的行为，我们只是想获得它的引用。

3.我们打算去利用一个特性，就是用装饰器函数的副产品去保存一个提供给我们的路径之间的链接，装饰器函数应该与它关联起来。
```python
class NotFlask():
    def __init__(self):
        self.routes = {}

    def route(self, route_str):
        def decorator(f):
            self.routes[route_str] = f
            return f
        return decorator

app = NotFlask()

@app.route("/")
def hello():
    return "Hello World!"
```
4.可如果没法访问内部的视图函数，保存路径的字典又有什么用？<br>
让我们加入一个方法serve(path)，当给定的路径存在时运行一个函数并给们我结果，当路径尚未注册时则抛出一个异常。
```python
class NotFlask():
    def __init__(self):
        self.routes = {}

    def route(self, route_str):
        def decorator(f):
            self.routes[route_str] = f
            return f
        return decorator

    def serve(self, path):
        view_function = self.routes.get(path)
        if view_function:
            return view_function
        else:
            raise ValueError('Route "{}"" has not been registered'.format(path))

app = NotFlask()

@app.route("/")
def hello():
    return "Hello World!"
```

##### 【挂钩‘serve’方法实现一个HTTP服务器】
```python
app = NotFlask()

@app.route("/")
def hello():
    return "Hello World!"

print app.serve("/")
```
