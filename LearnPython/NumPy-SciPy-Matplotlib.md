## Python 核心扩展库 NumPy SciPy Matplotlib

#### 扩展库的演变和功能
* **NumPy** 支持大量的维度数组与矩阵运算，是 Python 的数值数学扩展包。
* **SciPy** 为 NumPy 进一步提供算法库和数学工具包。``包含最优化、线性代数、积分、插值、特殊函数、快速傅里叶变换、信号处理和图像处理、常微分方程求解和其他科学与工程中常用的计算。``
* **Matplotlib** 则为 NumPy 提供可视化操作界面。
* **SciKit-Learn** 基于 SciPy 为 NumPy 更进一步的提供传统机器学习算法。
* **Tensorflow、PyTorch 等** 则基于 SciKit-Learn 和 SciPy 为 NumPy 提供更高级的深度学习算法。

**整个 NumPy 的核心模型是围绕一个 N 维数组对象 ndarray。**

#### 什么是 ndarray？
ndarray 是一系列同类型数据的集合，以 0 下标为开始进行集合中元素的索引。
* ndarray 对象是存放同类型元素的多维数组。
* ndarray 中的每个元素在内存中都有相同存储大小的区域。

ndarray 的几个重要属性
* dtype 对象中元素的类型
* ndim 维度的数量
* shape 数组的维度
* size 对象中元素的总个数

#### ndarray 的切边和索引
通过**冒号分隔**切片参数 ``start:stop:step`` 来进行切片操作。
```
import numpy as np
 
a = np.arange(10)  
b = a[2:7:2]   # 从索引 2 开始到索引 7 停止，间隔为 2
print(b)
```
