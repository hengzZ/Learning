## Performance Modeling

#### 1. 分析思路
Phase 1 —— Qualitative Analysis (定性分析)
* 数据采集工具 emon + sde
* 图表分析报告 （趋势图）

<div align="center"><img src="pics/qualitative-analysis-report.jpg" width="50%"></div>

Phase 2 —— Quantitative Analysis (定量分析)
* 确定模型
* 参数求解，计算影响因子。

#### 2. 量化分析工具选择
* numpy + scipy
* scikit-learn
* tensorflow

##### 2.1 numpy + scipy
—— Scientific Computing Tools for Python.

常用组合
* **Numpy**， 数值计算(numerical computation)的基础工具包。
* **SciPy**， a collection of numerical algorithms（数值算法） and domain-specific toolboxes（特定领域的工具箱）, including signal processing, optimization, statistics and much more.
* **Matplotlib**， a mature and popular plotting package. 成熟且流行的绘图包。

SciPy 用户手册 <br>
https://docs.scipy.org/doc/scipy/reference/

##### 2.2 scikit-learn （推荐）
—— Machine Learning in Python.

* 工具定位 <br>
data mining and data analysis. 数据挖掘和数据分析。

* **Built on NumPy, SciPy, and matplotlib.**

一些应用案例 <br>
https://scikit-learn.org/stable/auto_examples/index.html

教程及 API 手册
* https://scikit-learn.org/stable/tutorial/index.html （机器学习，统计学习，新闻分类）
* https://scikit-learn.org/stable/user_guide.html  (内包含完善的机器学习知识结构，推荐阅读。)
* https://scikit-learn.org/stable/modules/classes.html

Scikit-Learn 的一些不成文规定（惯例）
* https://scikit-learn.org/stable/tutorial/basic/tutorial.html#conventions （注意查看）

##### 2.3 tensorflow
—— An end-to-end open source machine learning platform.

https://tensorflow.google.cn/overview

构成
* **High Level APIs**
    * **Keras**, TensorFlow's high-level API for building and training deep learning models.
    * **Eager Execution**, an API for writing TensorFlow code imperatively, like you would use Numpy.
    * **Importing Data**, easy input pipelines to bring your data into your TensorFlow program.
    * **Estimators**, a high-level API that provides fully-packaged models ready for large-scale training and production.
* **Low Level APIs**
    * **Tensors**, which explains how to create, manipulate, and access Tensors--the fundamental object in TensorFlow.
    * **Variables**, which details how to represent shared, persistent state in your program.
    * **Graphs**, dataflow graphs, which are TensorFlow's representation of computations as dependencies between operations.
    * **Sessions**, which are TensorFlow's mechanism for running dataflow graphs across one or more local or remote devices.
* Estimators （预定义模型）
    * **Premade Estimators**(已有模型), the basics of premade Estimators.
    * **Checkpoints**(中间结果备份), save training progress and resume where you left off.
    * **Feature Columns**(原始数据与模型中 tensor 的对应), handle a variety of input data types without changes to the model.
    * **Datasets for Estimators**(输入), use ``tf.data`` to input data.
    * **Creating Custom Estimators**(自定义模型), write your own Estimator.
* Accelerators
    * Using GPUs
    * Using TPUs

<br>

#### Machine Learning v.s. Statistical Learning
##### 1. 机器学习
一般的，一个学习问题首先有一个样本集，希望做的是预测未知数据的特性。

学习问题进一步细分为以下几类
* 监督学习 —— 典型： 分类、回归。
* 无监督学习 —— 典型： clustering、density estimation（密度估计）、visualization（可视化）。

说明：
* 有监督与无监督的区别在于，是否每个训练集输入特征向量 x 都提供一个对应的 target value。
* 聚类问题的核心是分组，最直观的例子是直方图。
* 密度估计的核心是数据的分布特性，最直观的例子是高斯分布曲线图。 （高斯分布的直方图版本是否同时说明，探索分布先要分组？）
* 可视化？ —— 即 将数据从高维空间投影到二维或三维。 因为只有二维或三维才可视。

<div align="center"><img src="pics/Gaussian-distribution.jpg" width="35%"></div>

##### 2. 统计学习
机器学习是一种技术。 统计学习指使用机器学习技术，同时以统计预测(Statistical inference)为目标。 是机器学习在应用上的细分。

**Statistical Inference** 的明确含义
* 使用数据分析理论，推导潜在概率分布。
* 关键词： data analysis, distribution, probability.

说明
* 何为概率分布？ 先有分布直方图，后得每个分组的概率。

<br>

#### 确定所求问题的位置
##### 1. 需求
* 可视化 （report）
* 预测 -> 监督学习
* 分布图？

<br>

### Scikit-Learn
—— User Guide, https://scikit-learn.org/stable/user_guide.html

#### 1. 监督学习 (Supervised Learning)
##### 1.1 Generalized Linear Models (GLM) 广义线性模型
##### 1.1.1 数据模型 / 基础模型
<div align="center"><img src="pics/generalized-linear-models.jpg" width="25%"></div>

* 其中， x1 到 xp 可以是独立无关的特征，也可以是有关的特征，比如 xp = x1^2。
* 因此，可以使用 GLM 求解多项式方程拟合，需要事先计算 x^2, x^3， x^4... 然后带入求参即可。

##### 1.1.2 广义线性模型的引申用法（发展）
* Ordinary Least Squares 普通最小二乘法
  <div align="center"><img src="pics/GLM-ordinary-least-squares.jpg" width="10%"></div>
```bash
### 模块 - sklearn.linear_model.LinearRegression()
>>> from sklearn import linear_model
>>> reg = linear_model.LinearRegression()
>>> reg.fit([[0, 0], [1, 1], [2, 2]], [0, 1, 2])
...                                       
LinearRegression(copy_X=True, fit_intercept=True, n_jobs=None,
                 normalize=False)
>>> reg.coef_
array([0.5, 0.5])
``` 
* Ridge Regression 岭回归
  <div align="center"><img src="pics/GLM-ridge-regression.jpg" width="15%"></div>
```bash
### 模块 - sklearn.linear_model.Ridge()
>>> from sklearn import linear_model
>>> reg = linear_model.Ridge(alpha=.5)
>>> reg.fit([[0, 0], [0, 0], [1, 1]], [0, .1, 1]) 
Ridge(alpha=0.5, copy_X=True, fit_intercept=True, max_iter=None,
      normalize=False, random_state=None, solver='auto', tol=0.001)
>>> reg.coef_
array([0.34545455, 0.34545455])
>>> reg.intercept_ 
0.13636...
```
* Lasso 拉索回归
  <div align="center"><img src="pics/GLM-lasso-regression.jpg" width="23%"></div>
```bash
### 模块 - sklearn.linear_model.Lasso()
>>> from sklearn import linear_model
>>> reg = linear_model.Lasso(alpha=0.1)
>>> reg.fit([[0, 0], [1, 1]], [0, 1])  
Lasso(alpha=0.1, copy_X=True, fit_intercept=True, max_iter=1000,
   normalize=False, positive=False, precompute=False, random_state=None,
   selection='cyclic', tol=0.0001, warm_start=False)
>>> reg.predict([[1, 1]])
array([0.8])
```
* Elastic-Net 弹性网络
  <div align="center"><img src="pics/GLM-elastic-net.jpg" width="29%"></div>
```bash
### 模型 - sklearn.linear_model.ElasticNet()
```
* Least Angle Regression (LARS) 最小角回归法
* LARS Lasso (使用 LARS 求解 Lasso)
* Orthogonal Matching Pursuit (OMP) 正交匹配追踪
* Bayesian Regression 贝叶斯回归
  <div align="center"><img src="pics/GLM-bayesian-regression.jpg" width="13%"></div>
* Automatic Relevance Determination (ARD) 自动相关确定
* Logistic regression 逻辑斯蒂回归
  <div align="center"><img src="pics/GLM-logistic-regression.jpg" width="40%"></div>
* Stochastic Gradient Descent （SGD） 随机梯度下降
* Perceptron 感知机
* Passive Aggressive Algorithms （PA） 
* Robustness regression: outliers and modeling errors
    * RANSAC: RANdom SAmple Consensus
    * Theil-Sen estimator: generalized-median-based estimator
    * Huber Regression
 
 * Polynomial regression: extending linear models with basis functions
 ```bash
 ### 模块 - sklearn.preprocessing.PolynomialFeatures
 # 用于生成多项式模型中的高次项的值。 便于数据生成。 例如：
 >>> from sklearn.preprocessing import PolynomialFeatures
>>> import numpy as np
>>> X = np.arange(6).reshape(3, 2)
>>> X
array([[0, 1],
       [2, 3],
       [4, 5]])
>>> poly = PolynomialFeatures(degree=2)
>>> poly.fit_transform(X)
array([[ 1.,  0.,  1.,  0.,  0.,  1.],
       [ 1.,  2.,  3.,  4.,  6.,  9.],
       [ 1.,  4.,  5., 16., 20., 25.]])
 ```

##### 1.2 Linear and Quadratic Discriminant Analysis 线性与二次 判别分析

##### 1.3 Kernel ridge regression 核化岭回归

##### 1.4 Support Vector Machines (SVM) 支持向量机

##### 1.5 Stochastic Gradient Descent (SGD) 随机梯度下降

......
