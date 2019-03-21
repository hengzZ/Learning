## boost.python 实现 Python 与 C++ 混合编程


#### 1. Boost/Python 环境安装

##### 1.1 Boost 安装
```bash
# 使用包管理工具安装 （ubuntu）
sudo apt-get install --no-install-recommends libboost-all-dev
# （centos 7）
sudo yum install boost-devel

# 从源码编译安装
1. 下载压缩包 boost_1_64_0.tar.bz2 （https://dl.bintray.com/boostorg/release/1.64.0/source/）
   wget https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.bz2
2. 编译
   tar --bzip2 -xvf boost_1_64_0.tar.bz2
   cd boost_1_64_0/
   ./bootstrap.sh
   ./b2
3. 安装 （/usr/local/include/boost /usr/local/lib）
   sudo ./b2 install
4. 卸载
   sudo rm –rf /usr/local/include/boost
   sudo rm –rf /usr/local/lib/libboost_*
```

##### 1.2 Python 开发环境安装
```bash
# 使用包管理工具安装 （ubuntu）
sudo apt-get install python-dev
# (centos 7)
yum search python |grep devel
sudo yum install python-devel.x86_64
```


##### 1.3 Boost.Python wrapper 环境测试
* source code
```cpp
/* hello_ext.cpp */
#include <boost/python.hpp>

char const* greet()
{
   return "hello, world";
}

BOOST_PYTHON_MODULE(hello_ext)
{
    using namespace boost::python;
    def("greet", greet);
}
```
* 编译
```bash
# boost 安装目录： /usr/include /usr/lib64
# python-dev 安装目录： /usr/include/python2.7 /usr/lib64
g++ -Wall -O3 -std=c++11 -shared -fPIC -I/usr/include/python2.7 -lboost_python main.cpp -o hello_ext.so
```
* test code
```python
import hello_ext
print hello_ext.greet()

[output]:
hello, world
```


#### 2. bjam 编译环境安装 (optional)
###### bjam 与 cmake 一样，同为编译工具。
```
# 从源码编译安装 boost，会默认安装 bjam。 （bjam 是 boost 的一部分）
方法一：
    使用包管理工具下载 boost-dev 环境
方式二：
    下载源码，编译，但不安装 boost。
```


#### 3. 使用 C++ 编写 Python 扩展模块

##### 预热： boost-python 实现 c++ python 交互之 list - vector 互转
###### 编写 wrapper API 时， 1. vector 数据需要被 vector_indexing_suite\<> 包裹； 2. python 数据转为 C++ 数据需要使用 boost::python::extract<> 函数。
* cpp code
```cpp
#include <iostream>
#include <vector>
#include <string>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


typedef std::vector<std::string> MyList;
class MyClass {
public:
  void myFuncDump()
  {
    for (int i=0; i<m_list.size(); i++)
    {
      std::cout << m_list[i];
    }
    std::cout << std::endl;
  }
  void myFuncSet(boost::python::list pylist)
  {
    m_list.clear();
    for (int i=0; i<len(pylist); i++)
    {
      m_list.push_back(boost::python::extract<std::string>(pylist[i]));
    }
  }
private:
  MyList m_list;
};


// Wrapper code

using namespace boost::python;

BOOST_PYTHON_MODULE(mymodule)
{
  class_<MyClass>("MyClass")
   .def("myFuncDump", &MyClass::myFuncDump)
   .def("myFuncSet", &MyClass::myFuncSet)
   ;
}
```
* build
```bash
g++ -Wall -O3 -std=c++11 -shared -fPIC -I/usr/include/python2.7 -lboost_python main.cpp -o mymodule.so
```
* test code
```python
from mymodule import MyClass

if __name__ == '__main__':
    myclass = MyClass()

    mylist = ['hello', ',', 'world']
    myclass.myFuncSet(mylist)
    myclass.myFuncDump()
```

参考手册： https://www.boost.org/doc/libs/1_61_0/libs/python/doc/html/tutorial/index.html

##### 3.1 编写 c++ demo
```cpp 

```

##### 3.2 编写 Boost.Python wrapper


##### 3.3 编译


##### 3.4 测试
