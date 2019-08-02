## MySQL Setup

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
##### 2.1 MySQL 开发库
* PyMySQL (Python 原生库)
```bash
  pip3 install PyMySQL==0.9.3
```
* Flask-SQLAlchemy （推荐结合 Flask 使用）
```bash
  pip3 install PyMySQL==0.9.3
  pip3 install Flask==1.0.3
  pip3 install Flask-SQLAlchemy==2.4.0
```

##### 2.2 PyMySQL 代码实现
参考 models.py 文件，实现自己的表单模型。 （数据模型）

特别说明：
* PyMySQL 的主要对象只有两个
    * **connect()** —— socket 的抽象，用于和数据库建立连接。
    * **cursor()** —— 用于和数据库交互。
* API 文档
    * https://pymysql.readthedocs.io/en/latest/modules/index.html

##### 2.3 SQLAlchemy 代码实现
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
