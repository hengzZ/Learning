# MySQL 的字符集编码问题

## 0.MySQL查看字符集设置
```sql
mysql> show variables like '%char%';
```
结果：
```
+--------------------------+-------------------------------------+------
| Variable_name | Value |......
+--------------------------+-------------------------------------+------
| character_set_client | utf8 |......            -- 客户端字符集
| character_set_connection | utf8 |......
| character_set_database | utf8 |......          -- 数据库字符集
| character_set_filesystem | binary |......
| character_set_results | utf8 |......
| character_set_server | utf8 |......            -- 服务器字符集
| character_set_system | utf8 |......
| character_sets_dir | D:\MySQL Server 5.0\share\charsets\ |......
+--------------------------+-------------------------------------+------
```

## 1.查看MySQL数据表（table）的字符集。
```sql
mysql> show table status from sqlstudy_db like '%countries%';
```
结果：
```
+-----------+--------+---------+------------+------+-----------------+------
| Name | Engine | Version | Row_format | Rows | Collation |......
+-----------+--------+---------+------------+------+-----------------+------
| countries | InnoDB | 10 | Compact | 11 | utf8_general_ci |......
+-----------+--------+---------+------------+------+-----------------+------
```

## 2.查看MySQL数据列（column）的字符集。
```sql
mysql> show full columns from countries;
```
结果：
```
+----------------------+-------------+-----------------+--------
| Field | Type | Collation | .......
+----------------------+-------------+-----------------+--------
| countries_id | int(11) | NULL | .......
| countries_name | varchar(64) | utf8_general_ci | .......
| countries_iso_code_2 | char(2) | utf8_general_ci | .......
| countries_iso_code_3 | char(3) | utf8_general_ci | .......
| address_format_id | int(11) | NULL | .......
+----------------------+-------------+-----------------+--------
```

## 3.查看当前安装的MySQL所支持的字符集。
```sql
mysql> show charset;
mysql> show char set;
```
结果：
```
+----------+-----------------------------+---------------------+--------+
| Charset | Description | Default collation | Maxlen |
+----------+-----------------------------+---------------------+--------+
| latin1 | cp1252 West European | latin1_swedish_ci | 1 |
| latin2 | ISO 8859-2 Central European | latin2_general_ci | 1 |
| ascii | US ASCII | ascii_general_ci | 1 |
| utf8 | UTF-8 Unicode | utf8_general_ci | 3 |
| binary | Binary pseudo charset | binary | 1 |
| gbk | GBK Simplified Chinese | gbk_chinese_ci | 2 |
| gb2312 | GB2312 Simplified Chinese | gb2312_chinese_ci | 2 |
+----------+-----------------------------+---------------------+--------+
```

<br>

# 修改表和字段的字符集

## 1.修改数据库的字符集
```sql
mysql> alter database name character set utf8;
```

## 2.修改表的字符集
```sql
alter table 表名 convert to character set gbk;
```

## 3.修改字段的字符集
```sql
alter table 表名 modify column `字段名` varchar(30) character set gbk not null;
```

## 4.添加表字段
```sql
alter table 表名 add column `字段名` varchar (20) character set gbk;
```

<br>

# 修改MySQL的字符集

## 1.查找MySQL的cnf文件的位置
```bash
find / -iname '*.cnf' -print
```
> 拷贝 small.cnf、my-medium.cnf、my-huge.cnf、my-innodb-heavy-4G.cnf 其中的一个到 /etc 下，命名为 ``my.cnf``。

## 2.修改my.cnf

在``[client]``下添加:
```
default-character-set=utf8
```

在``[mysql]``下添加:
```
default-character-set=utf8
```

在``[mysqld]``下添加:
```
default-storage-engine=INNODB
character-set-server=utf8
```

## 3.重新启动MySQL
```bash
# 查看已是否启动
$ systemctl status mysqld.service
$ systemctl stop mysqld.service
$ systemctl start mysqld.service
```

> 报错：Job for mysqld.service failed because the control process exited with error code. See "systemctl status mysqld.service" and "journalctl -xe" for details.

查看 /var/log/mysqld.log 日志文件，发现：
```
2020-04-03T08:43:53.082026Z 0 [Note] Plugin 'FEDERATED' is disabled.
mysqld: Table 'mysql.plugin' doesn't exist
2020-04-03T08:43:53.082235Z 0 [ERROR] Can't open the mysql.plugin table. Please run mysql_upgrade to create it.
```

#### 注意：这种错误的出现，基本上相当于要重新配置 MySQL 了。

> 'mysql.plugin'不存在的原因是因为新安装的mysql服务后，一般需要执行数据库初始化操作 ，从而生成与权限相关的表。
```bash
# 运行如下命令
$ /usr/bin/mysql_install_db --user=mysql
# 会提示：mysql_install_db is deprecated. Please consider switching to mysqld --initialize
# 先确认 /var/lib/mysql 目录不存在，如果存在的话，请先做好备份。
$ mysqld --initialize
# 注意：如果提示 --initialize specified but the data directory has files in it. Aborting.
# 说明有以往的数据存在，请先将目录下的数据完好备份。换一个目录名。
```

注意：不推荐这种方式。 采用下面的方式：
```bash
# 确认 var/lib/mysql 目录不存在，同时 systemctl stop mysqld.service 关闭 mysqld，然后执行以下命令：
$ systemctl start mysqld
```

#### 修改 mysql 密码
```bash
# 先查看临时密码
$ cat /var/log/mysqld.log | grep password
# 登陆，例如： mysql -uroot -p'oR4)VRe(qn/o'
# 修改密码
mysql> SET PASSWORD FOR 'root'@'localhost' = PASSWORD('root@123');
```

#### 修改密码安全等级
```bash
# 查看当前的安全等级
mysql> SHOW VARIABLES LIKE 'validate_password%';
# 修改安全等级和root密码
mysql> set global validate_password_policy=0;
mysql> set global validate_password_length=4;
mysql> alter user 'root'@'localhost' identified by 'root';
```
