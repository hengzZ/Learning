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

<br>

# Sample Code

``` python
import os, errno
import numpy as np
import pandas as pd
import datetime
import matplotlib.pyplot as plt
from scipy.stats import pearsonr


def mkdir_p(path):
    try:
        os.makedirs(path, exist_ok=True)
    except OSError as exc: #Python >2.5 (except OSError, exc: for Python <2.5)
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise

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

def ret_time_index(df):
    index = []
    for ts in df['ts']:
        time = time_formatter(ts)
        index.append(time)
    return pd.Series(index)

def dropna_in_ts(data):
    data = data[data['ts'].isnull().values == False]
    return data

def my_pearsonr(data):
    # PearsonR
    columns = [ col for col in data.columns]
    pccss = []
    for col in columns:
        print(col)
        x = data[col]
        y = data['uarch.cpu_ipc']
        print(type(x))
        print(type(y))
        pc = pearsonr(x, y)
        pccss.append(pc[0])
    return pd.Series(pccss, index=columns)


if __name__ == '__main__':
    csv_file = '/home/spark/nvme/Baidu/ps_instance_20200315.csv'
    mcsv_file = '/home/spark/nvme/Baidu/ps_machine_20200315.csv'

    df = pd.read_csv(csv_file)
    mdf = pd.read_csv(mcsv_file)

    df = dropna_in_ts(df)
    mdf = dropna_in_ts(mdf)

    change_index_by_ts(df)
    change_index_by_ts(mdf)

    df = df['03/15/2020':'03/16/2020']  #datetime filter
    mdf = mdf['03/15/2020':'03/16/2020']


    #machine_table = df.groupby('machine').groups.keys()
    #service_table = df.groupby('service').groups.keys()
    #container_table = df.groupby('container').groups.keys()


    # TEST
    # bjhw-ps-wwwsug9.bjhw pisces14.www 1000003.pisces14.www.hba
    #data = df.groupby('machine').get_group('bjhw-ps-wwwsug9.bjhw').groupby('service').get_group('pisces14.www').groupby('container').get_group('1000003.pisces14.www.hba')
    #change_index_by_ts(data)
    #print(data)
    #data.to_csv('data.csv')


    # DATA SPLITING
    #root = os.path.basename(csv_file).split('.csv')[0]
    #print('********** processing... ', root, ' ***********')

    #mgroups = df.groupby('machine')
    #for machine in mgroups.groups.keys():

    #    mkdir_p(os.path.join(root,machine))
    #    sgroups = mgroups.get_group(machine).groupby('service')

    #    for service in sgroups.groups.keys():

    #        cgroups = sgroups.get_group(service).groupby('container')

    #        for container in cgroups.groups.keys():

    #             filename = service + '__' + container + '.csv'
    #             filepath = os.path.join(root, machine, filename)

    #             data = cgroups.get_group(container)
    #             data = data[data['ts'].isnull().values == False]

    #             change_index_by_ts(data)
    #             data.to_csv(filepath)
    #print('********** done ***********')


    # PearsonR
    service_groups = df.groupby('service')
    for service in service_groups.groups.keys():

        pearson_report = None
        #service_list = ['pandora-agent-hba.www', 'bs_vip', 'bs_ext', 'bs_se']
        #service_list = ['bs_se']
        service_list = ['bs_vip']
        if service not in service_list:
            continue

        container_groups = service_groups.get_group(service).groupby('container')

        for container in container_groups.groups.keys():

            data = container_groups.get_group(container)
            machine = list(data.groupby('machine').groups.keys())[0]
            mdata = mdf.groupby('machine').get_group(machine)
            other_services = df.groupby('machine').get_group(machine)

            # module level data
            data['uarch.mem_stall_cycles / uarch.cpu_cycles'] = data['uarch.mem_stall_cycles'] / data['uarch.cpu_cycles']
            data['Port 0,1,5,6 utilization'] = data['uarch.pipeline_port0_utilization'] + data['uarch.pipeline_port1_utilization'] + data['uarch.pipeline_port5_utilization'] + data['uarch.pipeline_port6_utilization']
            data['Port 2,3,4,7 utilizaiton'] = data['uarch.pipeline_port2_utilization'] + data['uarch.pipeline_port3_utilization'] + data['uarch.pipeline_port4_utilization'] + data['uarch.pipeline_port7_utilization']

            data = data[['uarch.cpu_ipc', 'uarch.tlb_itlb_mpki', 'uarch.tlb_dtlb_load_mpki', 'uarch.l1c_mpki', 'uarch.l1d_mpki', 'uarch.l3_mpki', 'uarch.l2_mpki', 'uarch.smt_pct_1smt_active', 'numa.mem_bw_local_ratio', 'numa.mem_bw_total', 'uarch.mem_load_per_instr', 'uarch.mem_store_per_instr', 'uarch.mem_stall_cycles / uarch.cpu_cycles', 'Port 0,1,5,6 utilization', 'Port 2,3,4,7 utilizaiton']]

            # server level data
            mdata['uarch.mem_stall_cycles / uarch.cpu_cycles'] = mdata['uarch.mem_stall_cycles'] / mdata['uarch.cpu_cycles']
            mdata['Port 0,1,5,6 utilization'] = mdata['uarch.pipeline_port0_utilization'] + mdata['uarch.pipeline_port1_utilization'] + mdata['uarch.pipeline_port5_utilization'] + mdata['uarch.pipeline_port6_utilization']
            mdata['Port 2,3,4,7 utilizaiton'] = mdata['uarch.pipeline_port2_utilization'] + mdata['uarch.pipeline_port3_utilization'] + mdata['uarch.pipeline_port4_utilization'] + mdata['uarch.pipeline_port7_utilization']

            mdata = mdata[['uarch.cpu_ipc', 'uarch.cpu_util', 'uarch.l3_mpki', 'uarch.smt_pct_1smt_active', 'uarch.mem_stall_cycles / uarch.cpu_cycles', 'Port 0,1,5,6 utilization', 'Port 2,3,4,7 utilizaiton', 'numa.mem_bw_total']]

            # server level data columns rename
            mdata.rename(columns={'uarch.cpu_ipc': 'server.uarch.cpu_ipc', 'uarch.cpu_util': 'server.uarch.cpu_util', 'uarch.l3_mpki': 'server.uarch.l3_mpki', 'uarch.smt_pct_1smt_active': 'server.uarch.smt_pct_1smt_active', 'uarch.mem_stall_cycles / uarch.cpu_cycles': 'server.uarch.mem_stall_cycles / uarch.cpu_cycles', 'Port 0,1,5,6 utilization': 'server.Port 0,1,5,6 utilization', 'Port 2,3,4,7 utilizaiton': 'server.Port 2,3,4,7 utilizaiton', 'numa.mem_bw_total': 'server.numa.mem_bw_total'}, inplace=True)


            # other modules data (cpu util)
            osdata = None
            other_service_groups = other_services.groupby('service')
            for s in other_service_groups.groups.keys():
                s_service_containers = other_service_groups.get_group(s).groupby('container')
                for c in s_service_containers.groups.keys():
                    ssdata = s_service_containers.get_group(c)

                    if s == service and c == container:
                        continue

                    print('=== machine: === ', machine, ' === container: === ', c, ' === service: === ', s)
                    # cpu util (other module)
                    cpu_util = ssdata['uarch.cpu_util']
                    # resample to 5mins
                    cpu_util = cpu_util.resample('5T').mean()
                    # data column rename
                    newname = machine + '.' + c + '.' + s + '.uarch.cpu_util'
                    cpu_util.rename(newname, inplace=True)

                    # concat
                    if osdata is None:
                        osdata = cpu_util
                    else:
                        osdata = pd.concat([osdata, cpu_util], axis=1)


            #DATA DUMP
            #if osdata is not None:
                #osdata.to_csv(service + '__other_modules_cpu_util.csv')
            #data.to_csv(service + '__instance_rawdata.csv')
            #mdata.to_csv(service + '__machine_rawdata.csv')
            #other_services.to_csv(service + '__other_services_rawdata.csv')

            # resample to 5mins (same as other modules' data)
            data = data.resample('5T').mean()
            mdata = mdata.resample('5T').mean()

            # time slice
            data = data['03/15/2020 20:00:00':'03/15/2020 22:00:00']
            mdata = mdata['03/15/2020 20:00:00':'03/15/2020 22:00:00']
            if osdata is not None:
                osdata = osdata['03/15/2020 20:00:00':'03/15/2020 22:00:00']

            # MaxMinScalar
            data = (data-data.min())/(data.max()-data.min())
            mdata = (mdata-mdata.min())/(mdata.max()-mdata.min())
            if osdata is not None:
                osdata = (osdata-osdata.min())/(osdata.max()-osdata.min())

            # concat
            if osdata is not None:
                results = pd.concat([data, mdata, osdata], axis=1)
            else:
                results = pd.concat([data, mdata], axis=1)

            # drop and fill NaN
            results = results[results['uarch.cpu_ipc'].isnull().values == False]
            results = results.fillna(0)

            if len(results) < 2:
                print('[WARNNING] ====== PASSED!! SERVICE: ' + service + ' CONTAINER: ' + container + ' ======')
                continue

            # PearsonR
            pccss = my_pearsonr(results)
            pccss.rename(service+'.'+container, inplace=True)

            if pearson_report is None:
                pearson_report = pccss
            else:
                pearson_report = pd.concat([pearson_report, pccss], axis=1)


            #data.to_csv(service + '__' + container + '__' + machine + '__5min_instance_mean.csv')
            #mdata.to_csv(service + '__' + container + '__' + machine + '__5min_machine_mean.csv')
            #osdata.to_csv(service + '__' + container + '__' + machine + '__5min_other_services_mean.csv')
            #results.to_csv(service + '__' + container + '__' + machine + '__5min_results.csv')
            #pccss.to_csv(service + '__' + container + '__' + machine + '__5min_pearsonr.csv')

            #metric = 'uarch.cpu_ipc'  # uarch.cpu_util,uarch.cpu_ipc
            #data[metric].plot()
            #break

        #plt.savefig('figs/' + service + '_'+ metric + '.png')
        #plt.clf()  #Clear figure
        pearson_report.to_csv(service + '__ps_pearson_report_8_10.csv')
```
