# Pandas

基于 NumPy 的数据分析包，该工具为了解决数据分析任务而创建。

``` bash
pip install pandas
```

``` bash
import numpy as np
import pandas as pd


if __name__ == '__main__':

    df = pd.read_csv('data.csv')
    dir(df)

    for col in df.columns:
        series = df[col]
        # do something with series
```

—  User Guide: https://pandas.pydata.org/docs/user_guide/index.html

### Pandas 的数据结构

- Series
- DataFrame

Pandas 的主要数据结构是 Series（一维数据）与 DataFrame（二维数据），这两种数据结构足以处理金融、统计、社会科学、工程等领域里的大多数典型用例。

### Pandas 的数学模型/函数支持

- IO tools (text, CSV, HDF5, …)
- Indexing and selecting data
- MultiIndex / advanced indexing
- Merge, join, and concatenate
- Reshaping and pivot tables
- Working with text data
- Working with missing data
- Categorical data
- Visualization
- Computational tools
    - Statistical functions
    - Window Functions
    - Aggregation
    - Expanding windows
    - Exponentially weighted windows
- Group By: split-apply-combine
- Time series / date functionality
- Time deltas
- Styling
- Options and settings
- Scaling to large datasets
- Sparse data structures

Pandas 适用于处理以下类型的数据：
- 与 SQL 或 Excel 表类似的，含异构列的表格数据
- 有序和无序（非固定频率）的时间序列数据
- 带行列标签的矩阵数据，包括同构或异构型数据
- 任意其它形式的观测、统计数据集，数据转入 Pandas 数据结构时不必事先标记

快速入门 https://www.pypandas.cn/docs/getting_started/


# 手册

https://www.pypandas.cn/docs/getting_started/dsintro.html#series


#### 1. 缺失数据

DataFrame 里的缺失值用 np.nan 表示。

#### 2. 提取、添加、删除

``` ipython
In [61]: df['one']
Out[61]:
a    1.0
b    2.0
c    3.0
d    NaN
Name: one, dtype: float64

In [62]: df['three'] = df['one'] * df['two']

In [63]: df['flag'] = df['one'] > 2

In [64]: df
Out[64]:
   one  two  three   flag
a  1.0  1.0    1.0  False
b  2.0  2.0    4.0  False
c  3.0  3.0    9.0   True
d  NaN  4.0    NaN  False

# 删除测试
In [65]: del df['two']

In [66]: three = df.pop('three')

In [67]: df
Out[67]:
   one   flag
a  1.0  False
b  2.0  False
c  3.0   True
d  NaN  False
```

#### 3. 标量值以广播的方式填充列

```ipython
In [68]: df['foo'] = 'bar'

In [69]: df
Out[69]:
   one   flag  foo
a  1.0  False  bar
b  2.0  False  bar
c  3.0   True  bar
d  NaN  False  bar
```

#### 4. 数据分组、筛选

##### 测试数据
``` ipython
import pandas as pd

ipl_data = {'Team': ['Riders', 'Riders', 'Devils', 'Devils', 'Kings',
         'kings', 'Kings', 'Kings', 'Riders', 'Royals', 'Royals', 'Riders'],
         'Rank': [1, 2, 2, 3, 3,4 ,1 ,1,2 , 4,1,2],
         'Year': [2014,2015,2014,2015,2014,2015,2016,2017,2016,2014,2015,2017],
         'Points':[876,789,863,673,741,812,756,788,694,701,804,690]}
df = pd.DataFrame(ipl_data)
```

##### 分组
```ipython
# 以列名Team去分组
df.groupby('Team')

# 查看分组情况
df.groupby('Team').groups

# 按照多列分组
df.groupby(['Team', 'Year'])

# 查看分组情况
df.groupby(['Team', 'Year']).groups

# 遍历分组
for name,group in df.groupby('Year'):
    print (name)
    print (group)
# 选择某一个分组
temp = df.groupby('Year').get_group(2014)
```

##### 聚合
```ipython
# 以Team列分组
df.groupby('Team')

# 聚合后求每组后的大小
df.groupby('Team').agg(np.size)

# 针对分组后的列Points，聚合应用聚类函数，一次应用，多个聚合函数
agg = df.groupby('Team')['Points'].agg([np.sum, np.mean, np.std])
```

##### 转换
```ipython
grouped = df.groupby('Team')
score = lambda x: (x - x.mean()) / x.std()*10
grouped.transform(score)

# 时间格式转换
data = pd.read_excel("9月打卡记录.xlsx")
data['时间'] = pd.to_datetime(data["时间"], format="%H:%M:%S")
# 按姓名分组, 再对日期去重,保留第一个值
ret = data.groupby("姓名", as_index=False).apply(lambda df:df.drop_duplicates("日期"))
# 筛选日期,并保存为excel文件
ret[ret["时间"] >= "1900/1/1 09:00:00"].to_excel("9月迟到名单.xlsx", index=False)
```

##### 过滤
``` ipython
filter = df.groupby('Team').filter(lambda x: len(x) >= 3)
```
