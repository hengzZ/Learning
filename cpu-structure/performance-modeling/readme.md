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
* https://scikit-learn.org/stable/tutorial/index.html
* https://scikit-learn.org/stable/user_guide.html
* https://scikit-learn.org/stable/modules/classes.html

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

#### Scikit-Learn
