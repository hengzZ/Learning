## Python 核心扩展库 NumPy SciPy Matplotlib

### 1 扩展库的演变和功能
* **NumPy** 支持大量的维度数组与矩阵运算，是 Python 的数值数学扩展包。
* **SciPy** 为 NumPy 进一步提供算法库和数学工具包。``包含最优化、线性代数、积分、插值、特殊函数、快速傅里叶变换、信号处理和图像处理、常微分方程求解和其他科学与工程中常用的计算。``
* **Matplotlib** 则为 NumPy 提供可视化操作界面。
* **SciKit-Learn** 基于 SciPy 为 NumPy 更进一步的提供传统机器学习算法。
* **Tensorflow、PyTorch 等** 则基于 SciKit-Learn 和 SciPy 为 NumPy 提供更高级的深度学习算法。

**整个 NumPy 的核心模型是围绕一个 N 维数组对象 ndarray。**

### 2 什么是 ndarray？
ndarray 是一系列同类型数据的集合，以 0 下标为开始进行集合中元素的索引。
* ndarray 对象是存放同类型元素的多维数组。
* ndarray 中的每个元素在内存中都有相同存储大小的区域。

ndarray 的几个重要属性
* dtype 对象中元素的类型
* ndim 维度的数量
* shape 数组的维度
* size 对象中元素的总个数

### 3 ndarray 的切边和索引
通过**冒号分隔**切片参数 ``start:stop:step`` 来进行切片操作。
```
import numpy as np
 
a = np.arange(10)  
b = a[2:7:2]   # 从索引 2 开始到索引 7 停止，间隔为 2
print(b)
```

### 4 NumPy 迭代数组内的元素
NumPy 迭代器对象 numpy.nditer 提供了一种灵活访问一个或者多个数组元素的方式。 ``迭代器最基本的任务是可以完成对数组元素的访问。``

##### 4.1 一般情况
```python
import numpy as np

a = np.arange(6).reshape(2,3)
for x in np.nditer(a):
    print (x)
```

##### 4.2 遍历时控制遍历顺序
```python
# for x in np.nditer(a, order='F'):  Fortran order，即是列序优先；
# for x in np.nditer(a.T, order='C'):  C order，即是行序优先；
import numpy as np

a = np.arange(0,60,5).reshape(3,4)
for x in np.nditer(a, order =  'C'):
    print (x)
```

##### 4.3 修改数组中元素的值
nditer 对象有另一个可选参数 op_flags。 默认情况下，nditer 将视待迭代遍历的数组为只读对象（read-only）。为实现对数组元素值的修改，必须指定 read-write 或者 write-only 的模式。
```python
import numpy as np

a = np.arange(0,60,5).reshape(3,4)
for x in np.nditer(a, op_flags=['readwrite']):
    x[...]=2*x
print (a)
```

##### 4.4 使用外部循环
nditer 对象拥有另一个参数 flags。可以有如下取值，
* c_index 跟踪 C 顺序的索引
* f_index 跟踪 Fortran 顺序的索引
* multi-index 每次迭代可以跟踪一种索引类型
* external_loop 给出的值是具有多个值的一维数组，而不是零维数组

```python
import numpy as np

a = np.arange(0,60,5).reshape(3,4)
for x in np.nditer(a, flags =  ['external_loop'], order =  'F'):
   print (x, end=", " )

# 原始数组是：
[[ 0  5 10 15]
 [20 25 30 35]
 [40 45 50 55]]
# 修改后的数组是：
[ 0 20 40], [ 5 25 45], [10 30 50], [15 35 55],
```

##### 4.5 广播迭代
如果两个数组是可广播的，nditer 组合对象能够同时迭代它们。
```python
import numpy as np

a = np.arange(0,60,5).reshape(3,4)
b = np.array([1,  2,  3,  4], dtype =  int)

for x,y in np.nditer([a,b]):
    print ("%d:%d"  %  (x,y), end=", " )

# 第一个数组为：
[[ 0  5 10 15]
 [20 25 30 35]
 [40 45 50 55]]

# 第二个数组为：
[1 2 3 4]

# 修改后的数组为：
0:1, 5:2, 10:3, 15:4, 20:1, 25:2, 30:3, 35:4, 40:1, 45:2, 50:3, 55:4,
```

参考 http://www.runoob.com/numpy/numpy-tutorial.html NumPy | 菜鸟教程
