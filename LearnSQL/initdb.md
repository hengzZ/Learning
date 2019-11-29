## MySQL 数据库环境的初始化

#### 表单示例

Product 对象的 product 表单

| 序号 | 字段名称 | 字段类型 | 字段描述
| :-:  | :-:     | :-:     | :-: 
| 1   | id            | varchar(36)  | 无意义，主键 uuid
| 2   | productNum    | varchar(50)  | 产品编号，唯一，不为空
| 3   | productName   | varchar(50)  | 产品名称
| 4   | cityName      | varchar(50)  | 出发城市
| 5   | departureTime | timestamp    | 出发时间
| 6   | productPrice  | float        | 产品价格
| 7   | productDesc   | varchar(500) | 产品描述
| 8   | productStatus | int          | 状态（0 关闭 1 开启）

MySQL 的完整数据类型列表，参阅 http://www.runoob.com/mysql/mysql-data-types.html 。

#### 数据库创建

initdb.sql
```sql
-- 设置会话的字符集编码
set names utf8;

-- 删除用户 drop user 'peter'@'%';
-- 创建用户 peter
create user 'peter'@'%' identified by 'peter@root';

-- 显示当前的所有数据库
show databases;

-- 删除数据库 drop database petersdemo;
-- 创建数据库 petersdemo
create database if not exists petersdemo default charset utf8 COLLATE utf8_general_ci;

-- 授权
grant all on *.* to 'peter'@'%';
-- 授予用户 guest 仅查看的权限，并仅限于 petersdemo 这个数据库，用户不存在则创建
grant select on petersdemo.* to 'guest'@'%' identified by 'guest@petersdemo';
-- 权限更新
flush privileges;

-- 显示当前所有用户信息
select user, host from mysql.user;

-- 使用数据库 petersdemo
use petersdemo;

-- 创建表单前，先删除
drop table if exists product;
-- 创建表单 product，注意：mysql 在建表时不支持函数，因此，只能在插入数据时使用 UUID()。
create table product (
    id varchar(36) NOT NULL PRIMARY KEY,
    productNum varchar(50) NOT NULL,
    productName varchar(50),
    cityName varchar(50),
    departureTime timestamp,
    productPrice float,
    productDesc varchar(500),
    productStatus int,
    CONSTRAINT product UNIQUE (id, productNum)
);

-- 插入 mock 数据
insert into product (id, productnum, productname, cityname, departuretime, productprice, productdesc, productstatus)
values (UUID(), 'product001', '上海三日游', '上海', str_to_date('29-11-2019 10:30:00.000000', '%d-%m-%Y %H:%i:%s.%f'), 1200, '魔都观光', 1);
insert into product (id, productnum, productname, cityname, departuretime, productprice, productdesc, productstatus)
values (UUID(), 'product002', '北京七日游', '北京', str_to_date('30-09-2019 09:00:00.000000', '%d-%m-%Y %H:%i:%s.%f'), 3500, '国庆七天乐', 0);
insert into product (id, productnum, productname, cityname, departuretime, productprice, productdesc, productstatus)
values (UUID(), 'product003', '上海五日游', '上海', str_to_date('15-01-2020 08:30:00.000000', '%d-%m-%Y %H:%i:%s.%f'), 2000, '魔都新春', 0);

-- 显示数据库的所有表单信息
show table status;

-- 显示表单的内容
select * from product;
```

MySQL 的访问权限、字段约束、日期模式、函数和操作符。
* 访问权限 http://www.mysqlzh.com/doc/44/175.html
* 字段约束 http://www.mysqlzh.com/doc/125/250.html
* 日期模式 http://www.mysqlzh.com/doc/118.html
* 函数和操作符 http://www.mysqlzh.com/doc/113.html

###### 补充： SQL 约束
```
约束用于限制加入表的数据的类型。 注意两点：
▪ 可以在创建表时规定约束（通过 CREATE TABLE 语句）。
▪ 也可以在表创建之后再规定约束（通过 ALTER TABLE 语句）。
# 主要的约束有（不是全部）:
* NOT NULL
* UNIQUE
* PRIMARY KEY
* FOREIGN KEY
* CHECK
* DEFAULT
```

#### MySQL 环境初始化
使用管理员账号登陆，source ``initdb.sql`` 脚本。
