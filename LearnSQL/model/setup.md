## Setup

##### 1. 执行 sql 脚本文件的方式
* Windows CMD 下
```bash
mysql -u账号 -p密码 -D数据库名 < sql文件绝对路径
```
* MySQL Client 下
```bash
mysql> source sql文件绝对路径
```

##### 2. 数据库安装
##### 2.1 MySQL Server Community 安装
```bash
下载 MySQL Community Server 5.7  链接：https://dev.mysql.com/downloads/mysql/5.7.html#downloads
* 安装模式设置 Setup Type - Server only
* 网络设置 Type and Networking - Server Machine
* 账号设置 Accounts and Roles - root@root
```
##### 2.2 基于 root 用户进行配置 // setup.sql
* 创建用户 info_connect@info_connect
* 创建数据库 info_connect
```sql
SHOW DATABASES;  # 查看所有数据库
CREATE DATABASE IF NOT EXISTS info_connect DEFAULT CHARSET UTF8 COLLATE UTF8_GENERAL_CI;

GRANT USAGE ON *.* TO 'info_connect'@'%' IDENTIFIED BY 'info_connect';  # 赋予用户仅查看的权限，用户不存在则创建
DROP USER 'info_connect'@'%';  # 删除用户
flush privileges;  # 
CREATE USER 'info_connect'@'%' IDENTIFIED BY 'info_connect';
GRANT ALL ON *.* TO 'info_connect'@'%';
flush privileges;

show databases;
select user,host from mysql.user;
```
运行
```bash
# 打开 mysql 客户端 （用 root 用户）， \s 查看字符集。 推荐 utf8 字符集配置。
# 在 mysql 客户端内运行
source C:\Users\zhihengw\PycharmProjects\learning\Learning\LearnSQL\model\setup.sql
```

<br>

#### Part 2 —— Model 模块开发
