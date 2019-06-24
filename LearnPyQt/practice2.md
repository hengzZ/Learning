## 应用程序架构模式 —— Model 模块

三种主要的架构模式
* MVC —— Model, View, ***Controller***(控制器)
* MVP —— Model, View, ***Presenter***(表示器)
* MVVM —— Model, View, ***ViewModel***(数据模型)

目的：
* ***Model*** 与 ***View*** 的解耦/交互。

<br>

#### 1. ***MVC、MVP、MVVM***
##### 1.1 ***MVC***
—— 客户端应用程序架构

MVC 的两种 invoke 方式
* **方式1** — View 接受指令，传递给 Controller，然后协调 Model 和 View。 （例如： 界面点击。）
* **方式2** — Controller 接受指令，传给 Controller，然后调用 Model 和 View。 （例如： F5 启动。）

MVC 的优缺点
* 优
    * 耦合性低
    * 重用性高
    * 生命周期成本低
    * 使开发和维护用户接口的技术含量降低
    * 可维护性高
* 缺
    * 花费大量时间将 MVC 应用到规模并不是很大的应用程序通常会得不偿失。
    * 视图与控制器间过于紧密连接，视图没有控制器的存在，其应用是很有限的，反之亦然。
    * 视图对 Model 的访问效率可能低效，例如对未变化数据的不必要的频繁访问。

##### 特别说明
```
1. MVC 中的 Controller 不是单独存在的，一般有大量的逻辑在 View 模块内。 (View Invoke 方式)
2. 另一些则是在 计时器事件 or 监听线程 等里面。 （Controller Invoke 方式）
（在 MVC 模式中，不存在独立的 Controller 模块。）
```

##### 1.2 ***MVP***
—— MVC 的改良模式

MVP 的特点
* View 非常薄，不部署任何业务逻辑，称为 ”被动视图”（Passive View）， 即没有任何主动性 Model 访问。
* Presenter 非常厚，所有逻辑都部署在里面。
* Presenter 与具体的 View 是没有直接关联的，而是通过定义好的接口(API)进行交互。

MVP 的优缺点
* 优
    * 模型与视图完全分离，不存在代码上的纠缠。
    * 可以更高效地使用 Model。
    * 可以将一个 Presenter 用于多个视图，复用性好。
    * 前后端分离后，可以脱离用户接口来测试这些逻辑。
* 缺
    * View 和 Presenter 的交互会过于频繁，View 和 Presenter 联系紧密。

##### 特别说明
```
1. 在 MVC 中，View 会直接访问 Model 读取数据而不是通过 Controller。
2. 在 MVP 中，View 并不直接使用 Model，它们之间的通信是通过 Presenter 来进行。
（在 MVP 模式中，有一个独立的 Presenter 模块存在。）
```

##### 1.3 ***MVVM***
—— 立足于原有 MVP 框架并且把 WPF 的新特性糅合进去。 (WPF，微软推出的基于 Windows 的用户界面框架)

新特性
* Binding（绑定）
* Dependency Property（依赖属性）
* Routed Events（路由事件）
* Command（命令）
* DataTemplate（数据模板）
* ControlTemplate（控制模板）等

VueJS 框架（MVVM 模式）中，VM 层为 Vue 内置，编码重点一部分在 View，一部分在 Model。 

<br>

#### 2. Model 模块编写

python 隔离环境
```bash
pip3 install virtualenv --proxy="server_url:port"
```

依赖库
* 方式1 Python 原生库
```bash
  pip3 install PyMySQL==0.9.3
```
* 方式2 Flask 组件库
```bash
  pip3 install PyMySQL==0.9.3
  pip3 install Flask==1.0.3
  pip3 install Flask-SQLAlchemy==2.4.0
```

MySQL Server Community 版本安装
```
下载 MySQL Community Server 5.7  链接：https://dev.mysql.com/downloads/mysql/5.7.html#downloads
安装模式设置 Setup Type - Server only
网络设置 Type and Networking - Server Machine
账号设置 Accounts and Roles - root@root
```

<br>

##### 2.1 PyMySQL 代码
—— https://pypi.org/project/PyMySQL/

数据库和表单创建
* 打开 MySQL Command Line Client 终端
* \s 打印状态
* \q 退出
* \h 帮助

创建命令
```bash
* 查看当前所有数据库
  show databases;
* 创建一个用户
  select user,host from mysql.user;
  create user 'username'@'%' identified by 'userpassword';
* 创建一个数据库
  CREATE DATABASE 数据库名称
  CREATE DATABASE IF NOT EXISTS 数据库名称 default charset utf8 COLLATE utf8_general_ci;
* 将数据库的访问权限授予某个用户
  grant all on *.* to 'username'@'%';
* 删除用户
  drop user 'username'@'%';
```

访问数据库
```bash
USE 库名;
```

删除命令
```bash
* 删除 Row / Record
  DELETE FROM 表单名称 WHERE id = 1;
* 删除表单
  DROP TABLE 表单名称;
* 删除数据库
  DROP DATABASE 数据库名称;
```

MySQL 客户端创建数据库
```bash
# 创建数据库 wechat_users
create database if not exists wechat_users default charset utf8;
show databases;
# 创建用户 wechat_user_accessor
create user 'wechat_user_accessor'@'%' identified by '111111';
# 为用户授权访问权限
## 模板： grant all privileges on 库名.表名 to '用户名'@'IP地址' identified by '密码' with grant option;
##       flush privileges;
grant all on wechat_users.* to 'wechat_user_accessor'@'%';
flush privileges;

use wechat_users;
\s
# 创建表单
CREATE TABLE `users` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `email` varchar(255) COLLATE utf8_bin NOT NULL,
    `password` varchar(255) COLLATE utf8_bin NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin
AUTO_INCREMENT=1;

# 查看表单信息
show table status;
```

python 数据库访问代码
```python
import pymysql.cursors

# 连接到数据库 wechat_user_accessor@localhost 
connection = pymysql.connect(host='localhost',
                             user='wechat_user_accessor',
                             password='111111',
                             db='wechat_users',
                             charset='utf8mb4',
                             cursorclass=pymysql.cursors.DictCursor)

try:
    with connection.cursor() as cursor:
        # 创建一个新的记录
        sql = "INSERT INTO `users` (`email`, `password`) VALUES (%s, %s)"
        cursor.execute(sql, ('helloworld@github.com', 'helloworld'))
        
        # connection is not autocommit by default. So you must commit to save
        # your changes.
        connection.commit()

    with connection.cursor() as cursor:
        # 查询记录
        sql = "SELECT `id`, `password` FROM `users` WHERE `email`=%s"
        cursor.execute(sql, ('helloworld@github.com',))
        result = cursor.fetchone()
        print(result)
finally:
    connection.close()
```

相关链接
1. SQL 语法， refer <br>
  [../LearnSQL](../LearnSQL)
2. MySQL Client/Server 模型， refer 《Chapter 14 MySQL Client/Server Protocol》 <br>
  https://dev.mysql.com/doc/internals/en/client-server-protocol.html

<br>

##### 2.2 SQLAlchemy 代码
—— http://www.pythondoc.com/flask-sqlalchemy/config.html

```python
import pymysql
from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
from werkzeug.security import generate_password_hash, check_password_hash

app = Flask(__name__)
# 指定用于访问的数据库
app.config["SQLALCHEMY_DATABASE_URI"] = "mysql+pymysql://wechat_user_accessor:111111@localhost/wechat_users"
# 追踪对象的修改并且发送信号
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = True
# 为 SQLAlchemy 指定一个应用程序（绑定方式）
db = SQLAlchemy(app)

"""
用户表
1.编号
2.邮箱
3.密码
"""
class User(db.Model):
    __tablename__ = "users"
    id = db.Column(db.Integer, nullable=False, autoincrement=True, primary_key=True) # 编号
    email = db.Column(db.String(255), nullable=False) # 邮箱
    password = db.Column(db.String(255), nullable=False) # 密码
    
    def __repr__(self):
        return "<User %r>" % self.email
    
    def check_pwd(self, pwd):
        return check_password_hash(self.password, pwd)

if __name__ == '__main__':
    #db.create_all() # 创建定义的表单
    user = User(
        email="sqlalchemy@github.com",
        password='sqlalchemy'
        #pwd=generate_password_hash("root")
        #addtime=datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    )
    db.session.add(user)
    db.session.commit()
    # 查询
    user = User.query.get(int(1))
    print(user)
    # 删除
    user = User.query.get(int(1))
    db.session.delete(user)  # 从数据库删除
    db.session.commit()
    # 再次查询
    users = User.query.all()
    if users:
        for user in users:
            print(user.id, user.email, user.password)
```

相关链接
* **COLLATE是用来做什么的？**
    * 可以理解为排序规则，简而言之，COLLATE 会影响到 ORDER BY 语句的顺序，会影响到 WHERE 条件中大于小于号筛选出来的结果。
    * https://www.cnblogs.com/qcloud1001/p/10033364.html
    
* **SQLAlchemy 表单模型定义**
    * http://www.pythondoc.com/flask-sqlalchemy/models.html （Flask Tutorial）
    * https://www.cnblogs.com/xushuhai/p/9101295.html (Column 详解)

* **应用程序与 SQLAlchemy 的协作详解** <br>
   http://www.pythondoc.com/flask-sqlalchemy/contexts.html
   * **方式1** 绑定
   ```python
     from flask import Flask
     from flask.ext.sqlalchemy import SQLAlchemy

     db = SQLAlchemy()

     def create_app():
         app = Flask(__name__)
         db.init_app(app)
         return app
      
     if __name__ == '__main__':
         db.create_all(app=create_app())  # 创建数据库的时候，必须指定一个 APP 对象
    ```
    * **方式2** 协作 （上下文context）
    ```python
      from flask import Flask
      from flask.ext.sqlalchemy import SQLAlchemy

      db = SQLAlchemy()

      def create_app():
          app = Flask(__name__)
          db.init_app(app)
          return app
      
      # 数据库访问函数 （方式1）
      def my_function():
          with app.app_context():
              user = db.User(...)
              db.session.add(user)
              db.session.commit()
   
       if __name__ == '__main__':
           app = create_app()
           app.app_context().push()
           user = db.User(...)
           db.session.add(user)
           db.session.commit()
           app.app_context().pop()
    ```
