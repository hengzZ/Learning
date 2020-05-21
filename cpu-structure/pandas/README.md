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


# Pandas 数据选取 df[] df.loc[] df.iloc[] df.ix[] df.at[] df.iat[]

#### 1 列选取

Dataframe 对象的每一列都有列名，可以通过列名实现对列的选取。列选取方式共有三种：标签索引、标签列表、Callable 对象。

``` ipython
# 标签索引：选取单个列
>>> df['name']
```

``` ipython
# 标签列表：选取多个列
>>> df[['name','age']]
```

``` ipython
# Callable 对象
# 选取第一列
>>> df[lambda df: df.columns[0]]
```

#### 2 行选取

选取行的方式包括三种：整数索引切片、标签索引切片和布尔数组。

``` ipython
>>> df

    name   age  gender isMarried
a    Joe  25.0       1       yes
b   Mike  32.0       0       yes
c   Jack  18.0       1        no
d   Rose   NaN       1       yes
e  David  15.0       0        no
f  Marry  20.0       1        no
g  Wansi  41.0       0        no
h   Sidy   NaN       0       yes
i  Jason  37.0       1        no
j   Even  32.0       0        no
```

``` ipython
# 整数索引切片：前闭后开 [)
# 选取第一行
>>> df[0:1]
# 选取前两行
>>> df[0:2]
```

``` ipython
# 标签索引切片：前闭后闭 []
# 选取第一行
>>> df[:'a']
# 选取前两行
>>> df['a':'b']
```

``` ipython
# 布尔数组
# 选取前三行
>>> df[[True,True,True,False,False,False,False,False,False,False]]
# 选取所有age大于30的行
>>> df[[each>30 for each in df['age']]]
# 通过布尔数组的方式，又可以衍生出下面的选取方式（选取所有age大于30的行）
>>> df[df['age']>30]
# 选取出所有age大于30，且isMarried为no的行
>>> df[(df['age']>30) & (df['isMarried']=='no')]
# 选取出所有age为20或32的行
>>> df[(df['age']==20) | (df['age']==32)]
```

#### 3 区域选取

采用 ``df.loc[] df.iloc[] df.ix[]`` 这三种方法进行数据选取时，方括号内必须有两个参数：第一个参数是对行的筛选条件，第二个参数是对列的筛选条件，两个参数用逗号隔开。

- df.loc[] 只能使用标签索引，不能使用整数索引，通过便签索引切边进行筛选时，前闭后闭。
- df.iloc[] 只能使用整数索引，不能使用标签索引，通过整数索引切边进行筛选时，前闭后开。
- df.ix[] 既可以使用标签索引，也可以使用整数索引。

#### 4 单元格选取

单元格选取包括 ``df.at[]`` 和 ``df.iat[]`` 两种方法。使用时必须输入两个参数，即行索引和列索引。其中，df.at[] 只能使用标签索引，df.iat[] 只能使用整数索引。 ``df.at[] 和 df.iat[] 选取的都是单个单元格（单行单列），所以返回值都为基本数据类型。``


# Pandas 按行按列遍历

遍历数据有以下三种方法:
- iteritems() 按列遍历，将 df 的每一列迭代为（列名，Series）对。
- iterrows() 按行遍历，将 df 的每一行迭代为（index，Series）对。
- itertuples() 按行遍历，将 df 的每一行迭代为元组，比 iterrows() 效率高。

``` ipython
for index, col in df.iteritems():
    print(index)

for index, row in df.iterrows():
    print(index)

for row in df.itertuples():
    print(index)
```

# 索引替换或添加新的列

``` ipython
def time_formatter(ts):
    # format: '%Y%m%d%H%M%S'
    tss = str(ts)
    year = int(tss[0:4])
    month = int(tss[4:6])
    days = int(tss[6:8])
    hours = int(tss[8:10])
    minutes = int(tss[10:12])
    seconds = int(tss[12:14])
    time = datetime.datetime(year, month, days) + datetime.timedelta(weeks=0, days=0, hours=hours, minutes=minutes,  seconds=seconds, milliseconds=0, microseconds=0)
    return time

def change_index_by_ts(df):
    index = []
    for ts in df['ts']:
        time = time_formatter(ts)
        index.append(time)
    df.index = pd.Series(index)
```

``` ipython
# 添加新的列

# 第一种直接赋值
df["newColumn"] = newValue
# 第二种用concat组合两个DataFrame
pd.concat([oldDf, newDf])

# 更改某一列的值

# 第一种，replace
df["column1"] = df["column1"].replace(oldValue, newValue)
# 第二种，map
df["column1"] = df["column1"].map({oldValue: newValue})
# 第三种，loc
# 将column2 中某些行（通过column1中的value1来过滤出来的）的值为value2
df.loc[df["column1"] == value1, "column2"] = value2

# 补全缺失值（数据清洗）

# fillna填充缺失值
df["column1"] = df["column1"].fillna(value1)
```
