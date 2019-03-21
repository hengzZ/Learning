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

##### 3.1 编写 c++ demo


##### 3.2 编写 Boost.Python wrapper


##### 3.3 编译


##### 3.4 测试
