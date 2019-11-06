## 联结表 （Join Tables）

#### 什么是联结表（Join Table）?
* 关系数据库，“关系” 二字的由来 —— 信息分解成多个表，一类数据一个表，各表通过某些共同的值互相关联。
* 初衷 —— 尽可能地减少数据冗余。 （相同的数据出现多次决不是一件好事。）
* 代价 —— 一个库中存在很多表，需要记忆表关系图。

###### 所谓多个表之间的关系，即联结（join）。
* SQL 的联结表概念： [联结表--最常用的-inner-join](readme.md#11-联结表--最常用的-inner-join)

#### 联结的种类 （4种）
* 内联结（又称等值联结） INNER JOIN
  ```
  # 格式： FROM table_name1, table_name2 WHERE ...
  SELECT prod_name, vend_name, prod_price, quantity
  FROM OrderItems, Products, Vendors
  WHERE Products.vend_id = Vendors.vend_id
    AND OrderItems.prod_id = Products.prod_id
    AND order_num = 20007;
  ```
* 自联结 （Self Join） 又名 “子查询”，同一个表内的两次递进查找。
  ```
  SELECT c1.cust_id, c1.cust_name, c1.cust_contact
  FROM Customers AS c1, Customers AS c2
  WHERE c1.cust_name = c2.cust_name
    AND c2.cust_contact = 'Jim Jones';
  ```
* 自然联结 （Natural Join） “严格约束的内联结” 作为联结的列一定是主键！
  ```
  # 格式： FROM table_name1, table_name2 WHERE ...
  SELECT prod_name, vend_name, prod_price, quantity
  FROM OrderItems, Products, Vendors
  WHERE Products.vend_id = Vendors.vend_id
    AND OrderItems.prod_id = Products.prod_id
    AND order_num = 20007;
  ```
* 外联结 （Outer Join） “自然联结还没联结上的时候就是外联结。”
  ```
  # Warning: 外联结一定要指定 LEFT 或 RIGHT，用于明确指定 “联结在左右哪个表中是主键”。
  SELECT Customers.cust_id, Orders.order_num
  FROM Customers LEFT OUTER JOIN Orders
    ON Customers.cust_id = Orders.cust_id;
  这样是为了更好的查询关联表，即使其中一个表还没有任何条目也不会报错。
  ```


## SQLAlchemy 中的 backref
SQL 中的联结只是在查询时的一种组合查询语法，在创建表的时候和常规的表创建没有任何区别。
但是，在 SQLAlchemy 中，为了支持联结这种查询，在创建表的时候需要显示将联结声明！！

#### 1. 没有声明联结时，SQLAlchemy 的组合查询方法
```python
def get_addresses_from_user(user_name):
    user = session.query(User).filter_by(name=user_name).first()
    addresses = session.query(Address).filter_by(user_id=user.id).all()
    return addresses
```

#### 2. 在创建表时，已声明联结。 此时可使用的查询语法
1. 使用 relationship 定义联结。
   ```python
   addresses = relationship('Address')
   ```
2. 直接在 User 对象中通过 addresses 属性获取对应的 Address 对象。
   ```python
   def get_addresses_from_user(user_name):
       user = session.query(User).filter_by(name=user_name).first()
       return user.addresses
   ```
3. backref，支持 Address 反向查询对应的 User。
   ```python
   addresses = relationship('Address', backref='user')
   ```
   ```python
   >>> a = Address()
   >>> a.user
   ```

#### 3. relationship + backref 使用示例
```python
# coding:utf8
import pymysql
from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
from werkzeug.security import generate_password_hash, check_password_hash

app = Flask(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = "mysql+pymysql://root:root@localhost/flask_study"
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = True
db = SQLAlchemy(app)

class User(db.Model):
    __tablename__ = "user"
    id = db.Column(db.Integer, primary_key=True)  # 编号
    name = db.Column(db.String(20), nullable=False)  # 账号
    pwd = db.Column(db.String(100), nullable=False)  # 密码
    addtime = db.Column(db.DateTime, nullable=False, index=True, default=datetime.now)  # 注册时间
    articles = db.relationship('Article', backref='user')  # 声明表关系。 注意，此处未指定联结！！

    def __repr__(self):
        return "<User %r>" % self.name

class Article(db.Model):
    __tablename__ = "article"
    id = db.Column(db.Integer, primary_key=True)  # 编号
    title = db.Column(db.String(255), nullable=False, unique=True)  # 标题
    logo = db.Column(db.String(255), nullable=False)  # 封面
    content = db.Column(db.Text, nullable=False)  # 内容
    addtime = db.Column(db.DateTime, nullable=False, index=True, default=datetime.now)  # 添加时间
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'))  # 为 user 与 article 的表关系指定联结！！ （自然联结）
```


## SQLAlchemy 多表操作
手册：
* 详细文档 https://www.osgeo.cn/sqlalchemy/
* 对象关系教程 https://www.osgeo.cn/sqlalchemy/orm/tutorial.html
