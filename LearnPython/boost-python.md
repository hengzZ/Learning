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
#include <Python.h>

// Produce deprecation warnings (needs to come before arrayobject.h inclusion).
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <numpy/arrayobject.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

// Temporary solution for numpy < 1.7 versions: old macro, no promises.
// You're strongly advised to upgrade to >= 1.7.
#ifndef NPY_ARRAY_C_CONTIGUOUS
#define NPY_ARRAY_C_CONTIGUOUS NPY_C_CONTIGUOUS
#define PyArray_SetBaseObject(arr, x) (PyArray_BASE(arr) = (x))
#endif

/* Fix to avoid registration warnings */
#define BP_REGISTER_SHARED_PTR_TO_PYTHON(PTR) do { \
  const boost::python::type_info info = \
    boost::python::type_id<shared_ptr<PTR > >(); \
  const boost::python::converter::registration* reg = \
    boost::python::converter::registry::query(info); \
  if (reg == NULL) { \
    bp::register_ptr_to_python<shared_ptr<PTR > >(); \
  } else if ((*reg).m_to_python == NULL) { \
    bp::register_ptr_to_python<shared_ptr<PTR > >(); \
  } \
} while (0)

namespace bp = boost::python;

// For Python, for now, we'll just always use float as the type.
typedef float Dtype;
const int NPY_DTYPE = NPY_FLOAT32;

// Check the PyArrary transformed from python into C++
void CheckContiguousArray(PyArrayObject* arr, std::string name,
    int batch, int channels, int height, int width) {
  if (!(PyArray_FLAGS(arr) & NPY_ARRAY_C_CONTIGUOUS)) {
    throw std::runtime_error(name + " must be C contiguous");
  }
  if (PyArray_NDIM(arr) != 4) {
    throw std::runtime_error(name + " must be 4-d");
  }
  if (PyArray_TYPE(arr) != NPY_FLOAT32) {
    throw std::runtime_error(name + " must be float32");
  }
  if (PyArray_DIMS(arr)[0] != batch) {
    throw std::runtime_error(name + " has wrong number of batch");
  }
  if (PyArray_DIMS(arr)[1] != channels) {
    throw std::runtime_error(name + " has wrong number of channels");
  }
  if (PyArray_DIMS(arr)[2] != height) {
    throw std::runtime_error(name + " has wrong height");
  }
  if (PyArray_DIMS(arr)[3] != width) {
    throw std::runtime_error(name + " has wrong width");
  }
}

// Convert bp object from list to vector
std::vector<std::string> ConvertBpListToVector(const bp::object& list)
{
  std::vector<std::string> string_vector;
  if (!list.is_none()) {
    for (int i = 0; i < len(list); i++) {
      string_vector.push_back(bp::extract<std::string>(list[i]));
    }
  }
  return string_vector;
}

// Convert bp object from python str to stl string
std::string ConvertBpStrToString(const bp::object& str)
{
  std::string ret_string;
  if (!str.is_none()) {
    ret_string = bp::extract<std::string>(str);
  }
  return ret_string;
}

// Copy bp array into c++ memory block
void SetInputArray(bp::object data_obj, int batch, int channels, int height, int width)
{
  // check that we were passed appropriately-sized contiguous memory
  PyArrayObject* data_arr = reinterpret_cast<PyArrayObject*>(data_obj.ptr());
  CheckContiguousArray(data_arr, "data array", batch, channels, height, width);
  // get the memory ptr
  Dtype* ptr = static_cast<Dtype*>(PyArray_DATA(data_arr));
  std::memset(ptr, 0, batch*channels*height*width*sizeof(Dtype));
}


BOOST_PYTHON_MODULE(mymodule)
{
  bp::def("set_input_array", &SetInputArray);
}
```

##### 3.2 编写 Boost.Python wrapper
```
Part I:
    编写 API， 以接收 or 返回 bp::object 对象， 实现交互。
Part II:
    导出接口
    BOOST_PYTHON_MODULE()
    {
      bp::def("func_name", &func_name);
      bp::class_<ClassName>("ClassName")
        .add_property("property_name", &ClassName::m_PropertyName)
        .def("method_name", &ClassName::MethodName)
    }
```

##### 3.3 编译
```bash
g++ -Wall -O3 -std=c++11 -shared -fPIC -I/usr/include/python2.7 -lboost_python main.cpp -o mymodule.so
```

##### 3.4 测试
```python
import numpy as np
from mymodule import set_input_array


if __name__ == '__main__':

    batch = 4
    channels = 3
    height = 299
    width = 299

    im = np.ones((batch,channels, height, width), dtype=np.float32)
    print(im.shape, im)
    set_input_array(im, batch, channels, height, width)
    print(im.shape, im)
```

###### 关于 Boost.Python 的复杂使用方法，请阅读 pycaffe 封装代码：
https://github.com/BVLC/caffe/blob/master/python/caffe/_caffe.cpp
