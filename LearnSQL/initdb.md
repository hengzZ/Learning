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

Order 对象的 order 表单

| 序号 | 字段名称 | 字段类型 | 字段描述
| :-:  | :-:     | :-:     | :-:
| 1   | id            | varchar(36)  | 无意义，主键 uuid
| 2   | orderNum      | varchar(50)  | 订单编号，唯一，不为空
| 3   | orderTime     | timestamp    | 下单时间
| 4   | peopleCount   | int          | 出行人数
| 5   | orderDesc     | varchar(500) | 订单描述（其他信息）
| 6   | payType       | int          | 支付方式（0 支付宝 1 微信 2 其他）
| 7   | orderStatus   | int          | 订单状态（0 未支付 1 已支付）
| 8   | productId     | varchar(36)  | 产品 id，外键
| 9   | memberId      | varchar(36)  | 会员（联系人） id，外键

User 对象 的 users 表单

| 序号 | 字段名称 | 字段类型 | 字段描述
| :-:  | :-:     | :-:     | :-:
| 1   | id       | varchar(36)  | 无意义，主键 uuid
| 2   | email    | varchar(50)  | 非空，唯一
| 3   | username | varchar(50)  | 用户名
| 4   | password | varchar(100) | 密码（加密）
| 5   | phoneNum | varchar(20)  | 电话
| 6   | status   | int          | 状态（0 未开启 1 开启）

Role 对象的 role 表单

| 序号 | 字段名称 | 字段类型 | 字段描述
| :-:  | :-:     | :-:     | :-:
| 1   | id       | varchar(36)  | 无意义，主键 uuid
| 2   | roleName | varchar(50)  | 角色名称
| 3   | roleDesc | varchar(50)  | 角色描述

Permission 对象的 permission 表单

| 序号 | 字段名称 | 字段类型 | 字段描述
| :-:  | :-:     | :-:     | :-:
| 1   | id             | varchar(36)  | 无意义，主键 uuid
| 2   | permissionName | varchar(50)  | 权限名
| 3   | url            | varchar(50)  | 资源路径

```java
/**
 * Product 对象
 */
public class Product {
    private String id;  // 主键
    private String productNum;  // 编号，唯一
    private String productName;  // 名称
    private String cityName;  // 出发城市
    @DateTimeFormat(pattern="yyyy-MM-dd HH:mm")
    private Date departureTime;  // 出发时间
    private String departureTimeStr;
    private Double productPrice;  // 商品单价
    private String productDesc;  // 商品描述
    private Integer productStatus;  // 状态: 0 关闭， 1 开启。
    private String productStatusStr;
}
/**
 * Order 对象
 */
public class Order {
    private String id;
    private String orderNum;
    @DateTimeFormat(pattern="yyyy-MM-dd HH:mm")
    private Date orderTime;
    private String orderTimeStr;
    private Integer peopleCount;
    private String orderDesc;
    private Integer payType;
    private String payTypeStr;
    private Integer orderStatus;
    private String orderStatusStr;
    private Product product;  // 一般是 List，此业务特殊，一个订单只能有一种产品
    private Member member;
    private List<Traveller> travellers;
}
/**
 * Member 对象
 */
public class Member {
    private String id;
    private String name;
    private String nickname;
    private String phoneNum;
    private String email;
}
/**
 * Traveller 对象
 */
public class Traveller {
    private String id;
    private String name;
    private String sex; // (male, female)
    private String phoneNum;
    private Integer credentialType;
    private String credentialTypeStr; // (0 身份证 1 学生证 2 军人证)
    private String credentialNum;
    private Integer travellerType; // (0 个人 1 团体)
    private String travellerTypeStr;
}
```

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

-- 创建表单前，先删除 (注意，先删除 “中间表”)
drop table if exists product;
drop table if exists member;
drop table if exists order_traveller;
drop table if exists traveller;
drop table if exists orders;
drop table if exists user_role;
drop table if exists role_permission;
drop table if exists users;
drop table if exists role;
drop table if exists permission;
-- 创建表单 product，注意：mysql 在建表时不支持函数，因此，只能在插入数据时使用 UUID()。
-- 另外注意，形如 CONSTRAINT product UNIQUE (id, productNum) 创建的是复合唯一性约束。
create table product (
    id varchar(36) NOT NULL PRIMARY KEY,
    productNum varchar(50) UNIQUE NOT NULL,
    productName varchar(50),
    cityName varchar(50),
    departureTime timestamp,
    productPrice float,
    productDesc varchar(500),
    productStatus int
);
-- 创建表单 member
create table member (
    id varchar(36) NOT NULL PRIMARY KEY,
    name varchar(50) NOT NULL,
    nickname varchar(50) UNIQUE NOT NULL,
    phoneNum varchar(50) UNIQUE NOT NULL,
    email varchar(50) UNIQUE
);
-- 创建表单 traveller
create table traveller (
    id varchar(36) NOT NULL PRIMARY KEY,
    name varchar(50) NOT NULL,
    sex varchar(50) NOT NULL,
    phoneNum varchar(50),
    credentialType int default 0,
    credentialNum varchar(50) NOT NULL,
    travellerType int default 1,
    CONSTRAINT traveller UNIQUE (credentialType, credentialNum)  -- 复合唯一性约束
);
-- 创建表单 orders (定义外键，同时注意外键的关联方式) 另注意，order 是 mysql 保留关键字。。改用 orders。
-- 参考 http://www.cnblogs.com/suguangti/p/10863867.html
create table orders (
    id varchar(36) NOT NULL PRIMARY KEY,
    orderNum varchar(50) UNIQUE NOT NULL,
    orderTime timestamp,
    peopleCount int,
    orderDesc varchar(500),
    payType int,
    orderStatus int,
    productId varchar(36),
    memberId varchar(36),
    foreign key(productId) references product(id) on update cascade on delete cascade,
    foreign key(memberId) references member(id) on update cascade on delete cascade
);
-- 创建表单 order_traveller (订单ID到Traveller列表的关联，“中间表”)
create table order_traveller (
    id varchar(36) NOT NULL PRIMARY KEY,
    orderId varchar(50) NOT NULL,
    travellerId varchar(50) NOT NULL,
    CONSTRAINT order_traveller UNIQUE (orderId, travellerId) -- 复合唯一性约束
);
-- 创建表单 users （user 为保留关键字，因此使用 users）
create table users (
    id varchar(36) NOT NULL PRIMARY KEY,
    email varchar(50) UNIQUE NOT NULL,
    username varchar(50) UNIQUE NOT NULL,
    password varchar(50) NOT NULL,
    phoneNum varchar(20) UNIQUE,
    status int
);
insert into users (id, email, username, password, phoneNum, status)
values (UUID(), 'root@petersdemo.com', 'root', 'root', '18800205200', 1);
insert into users (id, email, username, password, phoneNum, status)
values (UUID(), 'test@petersdemo.com', 'test', 'test', '18800205201', 1);
-- 创建表单 role
create table role (
    id varchar(36) NOT NULL PRIMARY KEY,
    roleName varchar(50) UNIQUE NOT NULL,
    roleDesc varchar(50)
);
insert into role (id, roleName, roleDesc) values (UUID(), 'role_admin', 'vip');
insert into role (id, roleName, roleDesc) values (UUID(), 'role_user', 'vip');
-- 创建表单 user_role （用户角色关联表 “中间表”）
create table user_role (
    userId varchar(36),
    roleId varchar(36),
    PRIMARY KEY (userId, roleId),  -- 定义复合主键约束
    FOREIGN KEY (userId) REFERENCES users(id),
    FOREIGN KEY (roleId) REFERENCES role(id)
);
declare @userId varchar(36);
declare @roleId varchar(36);
select @userId:=id from users where users.username = 'root';
select @roleId:=id from role where role.roleName = 'role_admin';
insert into user_role (userId, roleId) values (@userId, @roleId);
select @userId:=id from users where users.username = 'test';
select @roleId:=id from role where role.roleName = 'role_user';
insert into user_role (userId, roleId) values (@userId, @roleId);
-- 资源权限表 permission
create table permission (
    id varchar(36) NOT NULL PRIMARY KEY,
    permissionName varchar(50) UNIQUE NOT NULL,
    url varchar(50)
);
insert into permission (id, permissionName, url) values (UUID(), 'user findAll', '/user/findAll.do');
insert into permission (id, permissionName, url) values (UUID(), 'user findById', '/user/findById.do');
-- 创建表单 role_permission （角色权限关联表 “中间表”）
create table role_permission (
    roleId varchar(36),
    permissionId varchar(36),
    PRIMARY KEY (roleId, permissionId), -- 定义复合主键约束
    FOREIGN KEY (roleId) REFERENCES role(id),
    FOREIGN KEY (permissionId) REFERENCES permission(id)
);
declare @roleId varchar(36);
declare @permissionId varchar(36);
select @roleId:=id from role where role.roleName = 'role_admin';
select @permissionId:=id from permission where permission.permissionName = 'user findAll';
insert into role_permission (roleId, permissionId) values (@roleId, @permissionId);
select @permissionId:=id from permission where permission.permissionName = 'user findById';
insert into role_permission (roleId, permissionId) values (@roleId, @permissionId);  -- admin 有所有的 permission
select @roleId:=id from role where role.roleName = 'role_user';
select @permissionId:=id from permission where permission.permissionName = 'user findById';
insert into role_permission (roleId, permissionId) values (@roleId, @permissionId);


-- 修改 users.password 字段长度
-- 多个字段同时修改的写法： alter table 表名 modify column 字段名1 类型1, modify column 字段名2 类型2, modify column 字段名3 类型3;
alter table users modify column password varchar(100) NOT NULL;

-- 修改角色名称，与 Spring Security 配置文件中的 ROLE_USER, ROLE_ADMIN 一致
update role set roleName = 'ROLE_ADMIN' where roleName = 'role_admin';
update role set roleName = 'ROLE_USER' where roleName = 'role_user';

-- 修改用户的密码，与 Spirng Security 加密算法加密后的结果一致。否则导致添加加密功能后无法登陆。
update users set password = '$2a$10$QooiHVU7E4n/Eba/QKtfyef/Y/WRcyW70OojFE0mGoW79y.6YnTNi' where username = 'root';
update users set password = '$2a$10$b3atiIT6Szs1SzKsPLvDQ.JzywDMdNzz4qGN/Y9/8rYpsEIra9hOq' where username = 'test';


-- 插入 mock 数据
insert into product (id, productnum, productname, cityname, departuretime, productprice, productdesc, productstatus)
values (UUID(), 'product001', '上海三日游', '上海', str_to_date('29-11-2019 10:30:00.000000', '%d-%m-%Y %H:%i:%s.%f'), 1200, '魔都观光', 1);
insert into product (id, productnum, productname, cityname, departuretime, productprice, productdesc, productstatus)
values (UUID(), 'product002', '北京七日游', '北京', str_to_date('30-09-2019 09:00:00.000000', '%d-%m-%Y %H:%i:%s.%f'), 3500, '国庆七天乐', 0);
insert into product (id, productnum, productname, cityname, departuretime, productprice, productdesc, productstatus)
values (UUID(), 'product003', '上海五日游', '上海', str_to_date('15-01-2020 08:30:00.000000', '%d-%m-%Y %H:%i:%s.%f'), 2000, '魔都新春', 0);
-- 会员
insert into member (id, name, nickname, phoneNum, email)
values (UUID(), '开发者', 'peter', '18800205210', 'petersdemo@qq.com');
-- 订单 (在用户变量中保存值，将值从一个语句传递到另一个语句)
declare @productId varchar(36);
declare @memberId varchar(36);
select @productId:=id from product where product.productnum = 'product001';
select @memberId:=id from member where member.nickname = 'peter';
insert into orders (id, orderNum, orderTime, peopleCount, orderDesc, payType, orderStatus, productId, memberId)
values (UUID(), '#001', str_to_date('01-12-2019 12:00:00.000000', '%d-%m-%Y %H:%i:%s.%f'), 1, 'mock 数据', 0, 0, @productId, @memberId);
-- Traveller mock 数据
insert into traveller (id, name, sex, phoneNum, credentialType, credentialNum, travellerType)
values (UUID(), '开发者', '男', '18800205210', 0, '410105199003150306', 0);
-- 关联表 order_traveller mock 数据
declare @travellerId varchar(36);
declare @orderId varchar(36);
select @travellerId:=id from traveller where traveller.credentialType = 0 and traveller.credentialNum = '410105199003150306';
select @orderId:=id from orders where orders.orderNum = '#001';
insert into order_traveller (id, orderId, travellerId)
values (UUID(), @orderId, @travellerId);


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
