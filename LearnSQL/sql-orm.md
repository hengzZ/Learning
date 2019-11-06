## SQLite3

##### Modules
* SQLite3
* SQLAlchemy


### 1. 获取 Sql Session

#### 连接数据库
```python
import sqlite3

DATABASE = '/path/to/database.db'

def get_db():
    db = sqlite3.connect(DATABASE)
    return db

# 简化查询
# 行工厂（ row factory ）可以简化 SQLite 的使用，它会在每个结果返回的时候对返回结果进行加工。
def make_dicts(cursor, row):
    return dict((cursor.description[idx][0], value)
                for idx, value in enumerate(row))

# 1. 使 sqlite3 模块返回方便处理的字典类型的结果。
db.row_factory = make_dicts
# 2. 使查询返回 Row 对象，而不是字典。 （Row 对象是 namedtuple） 推荐！！
db.row_factory = sqlite3.Row  # 基于键的名称取值，如： r['FirstName']
```

#### 功能封装： 获得游标、执行查询和获取结果
```python
def query_db(query, args=(), one=False):
    cur = get_db().execute(query, args)
    rv = cur.fetchall()
    cur.close()
    return (rv[0] if rv else None) if one else rv
```

#### 演示示例
```python
# 1. 多行查询
for user in query_db('select * from users'):
    print user['username'], 'has the id', user['user_id']

# 2. 单一结果查询
user = query_db('select * from users where username = ?',
                [the_username], one=True)
if user is None:
    print 'No such user'
else:
    print the_username, 'has the id', user['user_id']
```
##### 注意： 如果要给 SQL 语句传递参数，请在语句中使用问号来代替参数，并把参数放在一个列表中一起传递。 （MyBatis 也是此用法。）

#### 补充： 数据库的所谓 “初始化模式”
关系数据库是需要模式的，因此一个应用常常需要一个 schema.sql 文件来创建数据库。
```python
def init_db():
    try:
        db = get_db()
        with open('schema.sql', mode='r') as f:
            db.cursor().executescript(f.read())
        db.commit()
    finally:
        db.close()
```


### 2. 映射器 (ORM)
SQLAlchemy 是一个常用的数据库抽象层，有四种常用方法。

#### 2.1 声明
```python
# 示例 database.py 模块
from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session, sessionmaker
from sqlalchemy.ext.declarative import declarative_base

engine = create_engine('sqlite:////tmp/test.db', convert_unicode=True)
db_session = scoped_session(sessionmaker(autocommit=False,
                                         autoflush=False,
                                         bind=engine))
Base = declarative_base()
Base.query = db_session.query_property()

def init_db():
    # 在这里导入定义模型所需要的所有模块，这样它们就会正确的注册在
    # 元数据上。否则你就必须在调用 init_db() 之前导入它们。
    import yourapplication.models
    Base.metadata.create_all(bind=engine)
```
##### 要定义模型的话，只要继承上面创建的 Base 类就可以了。
```python
# 示例模型（放入 models.py 中）
from sqlalchemy import Column, Integer, String
from yourapplication.database import Base

class User(Base):
    __tablename__ = 'users'
    id = Column(Integer, primary_key=True)
    name = Column(String(50), unique=True)
    email = Column(String(120), unique=True)

    def __init__(self, name=None, email=None):
        self.name = name
        self.email = email

    def __repr__(self):
        return '<User %r>' % (self.name)

# 使用 init_db 函数来创建数据库
>>> from yourapplication.database import init_db
>>> init_db()

# 插入条目
>>> from yourapplication.database import db_session
>>> from yourapplication.models import User
>>> u = User('admin', 'admin@localhost')
>>> db_session.add(u)
>>> db_session.commit()

# 查询条目
>>> User.query.all()
>>> User.query.filter(User.name == 'admin').first()
```

#### 2.2 人工对象关系映射 （Object <--> 关系型数据库） （重点内容）
人工对象关系映射相较于上面的声明方式有优点也有缺点。
* 所谓映射，即：分别定义表和类，并映射它们。
* 这种方式更灵活，但是要多些代码。

```python
# 示例 database.py 模块
from sqlalchemy import create_engine, MetaData
from sqlalchemy.orm import scoped_session, sessionmaker

engine = create_engine('sqlite:////tmp/test.db', convert_unicode=True)
metadata = MetaData()
db_session = scoped_session(sessionmaker(autocommit=False,
                                         autoflush=False,
                                         bind=engine))
def init_db():
    metadata.create_all(bind=engine)
```
##### 要定义模型的话，定义表和类，关联它们。
```python
# 示例模型（放入 models.py 中）
from sqlalchemy import Table, Column, Integer, String
from sqlalchemy.orm import mapper
from yourapplication.database import metadata, db_session

class User(object):
    query = db_session.query_property()

    def __init__(self, name=None, email=None):
        self.name = name
        self.email = email

    def __repr__(self):
        return '<User %r>' % (self.name)

users = Table('users', metadata,
    Column('id', Integer, primary_key=True),
    Column('name', String(50), unique=True),
    Column('email', String(120), unique=True)
)
mapper(User, users)

# 查询和插入与声明方式的一样。
>>> 略
```

#### 2.3 SQL 抽象层
如果你只需要使用数据库系统（和 SQL ）抽象层，那么基本上只要使用引擎。
```python
from sqlalchemy import create_engine, MetaData, Table

engine = create_engine('sqlite:////tmp/test.db', convert_unicode=True)
metadata = MetaData(bind=engine)

# 此时，如何使用数据库？ 要么像前文中一样在代码中声明表，要么自动载入它们。
from sqlalchemy import Table
users = Table('users', metadata, autoload=True)

# 插入条目
# （可以使用 insert 方法插入数据。为了使用事务，我们必须先得到一个连接）
>>> con = engine.connect()
>>> con.execute(users.insert(), name='admin', email='admin@localhost')
# 提醒： SQLAlchemy 会自动提交。


# 查询条目
# （可以直接使用引擎或连接来查询数据库，查询结果也是类字典元组）
>>> r = users.select(users.c.id == 1).execute().first()
>>> r['name']
```
注意： 也可以把 SQL 语句作为字符串传递给 execute() 方法。
```python
>>> engine.execute('select * from users where id = :1', [1]).first()
(1, u'admin', u'admin@localhost')
```

#### 2.4 Flask-SQLAlchemy 扩展
SQLAlchemy 是一个常用的数据库抽象层，因此其特点是需要一定的配置才能使用。 Flask-SQLAlchemy 做了一个处理 SQLAlchemy 的扩展，使开发者可以在 Flask 框架中配置 SQLAlchemy。


### 3. 项目布局
参考 Flask 大型应用布局 https://dormousehole.readthedocs.io/en/latest/patterns/packages.html#larger-applications
```
/home/user/Projects/flask-tutorial
├── flaskr/
│   ├── __init__.py
│   ├── db.py
│   ├── schema.sql
│   ├── auth.py
│   ├── blog.py
│   ├── templates/
│   │   ├── base.html
│   │   ├── auth/
│   │   │   ├── login.html
│   │   │   └── register.html
│   │   └── blog/
│   │       ├── create.html
│   │       ├── index.html
│   │       └── update.html
│   └── static/
│       └── style.css
├── tests/
│   ├── conftest.py
│   ├── data.sql
│   ├── test_factory.py
│   ├── test_db.py
│   ├── test_auth.py
│   └── test_blog.py
├── venv/
├── setup.py
└── MANIFEST.in
```

<br>

##### reference
* pysqlite3 https://dormousehole.readthedocs.io/en/latest/patterns/sqlite3.html
* pySQLAlchemy https://dormousehole.readthedocs.io/en/latest/patterns/sqlalchemy.html
* SQLite 手册 https://www.runoob.com/sqlite/sqlite-intro.html
* SQLAlchemy 中文手册 https://www.osgeo.cn/sqlalchemy/

## 关于 SQLAlchemy 中的 mapper() 函数

##### 1. 经典描述法
```python
#
# 定义两个表 User 和 Address，两个表的关系在 User 类定义（一对一关系）。
#
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String, ForeignKey

Base = declarative_base()

class User(Base):
    __tablename__ = 'user'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    fullname = Column(String)
    password = Column(String)

    addresses = relationship("Address", backref="user", order_by="Address.id")

class Address(Base):
    __tablename__ = 'address'

    id = Column(Integer, primary_key=True)
    user_id = Column(ForeignKey('user.id'))
    email_address = Column(String)
```

##### 2. 用 mapper() 来改写上述代码
```python
from sqlalchemy import Table, MetaData, Column, Integer, String, ForeignKey
from sqlalchemy.orm import mapper

metadata = MetaData()

user = Table('user', metadata,
            Column('id', Integer, primary_key=True),
            Column('name', String(50)),
            Column('fullname', String(50)),
            Column('password', String(12))
        )

address = Table('address', metadata,
            Column('id', Integer, primary_key=True),
            Column('user_id', Integer, ForeignKey('user.id')),
            Column('email_address', String(50))
            )

class User(object):
    def __init__(self, name, fullname, password):
        self.name = name
        self.fullname = fullname
        self.password = password

class Address(object):
    def __init__(self, user_id, email_address):
        self.user_id = user_id
        self.fullname = fullname
        self.email_address = email_address

mapper(User, user, properties={
    'addresses' : relationship(Address, backref='user', order_by=address.c.id)
})

mapper(Address, address)
```


## 示例
```python
# coding: utf-8

import random
from faker import Factory

from sqlalchemy import create_engine, Table
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import ForeignKey
from sqlalchemy import Column, String, Integer, Text
from sqlalchemy.orm import sessionmaker, relationship


engine = create_engine('mysql+pymysql://root@localhost:3306/blog?charset=utf8')
Base = declarative_base()


class User(Base):

    __tablename__ = 'users'

    id = Column(Integer, primary_key=True)
    username = Column(String(64), nullable=False, index=True)
    password = Column(String(64), nullable=False)
    email = Column(String(64), nullable=False, index=True)
    articles = relationship('Article', backref='author')
    userinfo = relationship('UserInfo', backref='user', uselist=False)

    def __repr__(self):
        return '%s(%r)' % (self.__class__.__name__, self.username)


class UserInfo(Base):

    __tablename__ = 'userinfos'

    id = Column(Integer, primary_key=True)
    name = Column(String(64))
    qq = Column(String(11))
    phone = Column(String(11))
    link = Column(String(64))
    user_id = Column(Integer, ForeignKey('users.id'))


class Article(Base):

    __tablename__ = 'articles'

    id = Column(Integer, primary_key=True)
    title = Column(String(255), nullable=False, index=True)
    content = Column(Text)
    user_id = Column(Integer, ForeignKey('users.id'))
    cate_id = Column(Integer, ForeignKey('categories.id'))
    tags = relationship('Tag', secondary='article_tag', backref='articles')

    def __repr__(self):
        return '%s(%r)' % (self.__class__.__name__, self.title)


class Category(Base):

    __tablename__ = 'categories'

    id = Column(Integer, primary_key=True)
    name = Column(String(64), nullable=False, index=True)
    articles = relationship('Article', backref='category')

    def __repr__(self):
        return '%s(%r)' % (self.__class__.__name__, self.name)


article_tag = Table(
    'article_tag', Base.metadata,
    Column('article_id', Integer, ForeignKey('articles.id')),
    Column('tag_id', Integer, ForeignKey('tags.id'))
)


class Tag(Base):

    __tablename__ = 'tags'

    id = Column(Integer, primary_key=True)
    name = Column(String(64), nullable=False, index=True)

    def __repr__(self):
        return '%s(%r)' % (self.__class__.__name__, self.name)


if __name__ == '__main__':
    Base.metadata.create_all(engine)

    faker = Factory.create()
    Session = sessionmaker(bind=engine)
    session = Session()

    faker_users = [User(
        username=faker.name(),
        password=faker.word(),
        email=faker.email(),
    ) for i in range(10)]
    session.add_all(faker_users)

    faker_categories = [Category(name=faker.word()) for i in range(5)]
    session.add_all(faker_categories)

    faker_tags= [Tag(name=faker.word()) for i in range(20)]
    session.add_all(faker_tags)

    for i in range(100):
        article = Article(
            title=faker.sentence(),
            content=' '.join(faker.sentences(nb=random.randint(10, 20))),
            author=random.choice(faker_users),
            category=random.choice(faker_categories)
        )
        for tag in random.sample(faker_tags, random.randint(2, 5)):
            article.tags.append(tag)
        session.add(article)

    session.commit()
```
