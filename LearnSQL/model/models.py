# coding: utf-8
import pymysql.cursors
from datetime import datetime


def connect():
    # Connect to the database
    client = pymysql.connect(host='localhost',
                             user='info_connect',
                             password='info_connect',
                             db='info_connect',
                             charset='utf8mb4',
                             cursorclass=pymysql.cursors.DictCursor)
    return client

def create_all():
    User._create_table()


"""
用户表： ID、微信ID（userID）、登录时间、登陆次数、服务类型、服务类型统计（1,2,3,4），备注（可能有teamviewer信息）
1. id (编号)
2. name (账号)
3. phoneNumber 电话号码
4. loginTime 注册时间
5. logonTime 登陆时间
6. service1 接受服务次数(统计)
   .......2
   .......3
   .......4
10. serviceType 请求服务类型
11. contact 联系方式
"""

class User(object):
    def __init__(self, name, phoneNumber, logonTime, serviceType, contact):
        self.name = name
        self.phoneNumber = phoneNumber
        self.logonTime = logonTime
        self.serviceType = serviceType
        self.contact = contact

    @staticmethod
    def _create_table():
        """
        # 查看表的字段信息：
        1. desc 表单名;
        2. show columns from 表单名;
        # 删除表：
        1. drop table 表单名;
        """
        client = connect()
        try:
            with client.cursor() as cursor:
                sql = """
                      CREATE TABLE IF NOT EXISTS `users` (
                        `id`           int(11)        UNSIGNED NOT NULL AUTO_INCREMENT,
                        `name`         char(64)       NOT NULL COLLATE utf8_bin,
                        `phoneNumber`  char(11)       DEFAULT NULL,
                        `loginTime`    datetime       NOT NULL,
                        `logonTime`    datetime       NOT NULL,
                        `service1`     int(11)        UNSIGNED DEFAULT 0,
                        `service2`     int(11)        UNSIGNED DEFAULT 0,
                        `service3`     int(11)        UNSIGNED DEFAULT 0,
                        `service4`     int(11)        UNSIGNED DEFAULT 0,
                        `serviceType`  char(11)       NOT NULL COLLATE utf8_bin,
                        `contact`      varchar(255)   NOT NULL COLLATE utf8_bin,
                        PRIMARY KEY (`id`)
                      )
                      """
                cursor.execute(sql)
                client.commit()
        finally:
            client.close()
    
    @staticmethod
    def fetch_info(id):
        print('fetch_info')

    def commit(self):
        """
        # 查看表单内容：
        1. select * from 表单名;
        """
        client = connect()
        try:
            with client.cursor() as cursor:
                sql = """
                      SELECT `id`, `name` FROM `users` WHERE `name`=%s
                      """
                cursor.execute(sql, (self.name,))
                result = cursor.fetchone()
                if result is None:  # new user
                    sql = """
                          INSERT INTO `users` (`name`, `loginTime`, `logonTime`, `serviceType`, `contact`) VALUES (%s, STR_TO_DATE(%s, '%%Y-%%m-%%d %%H:%%i:%%s'), STR_TO_DATE(%s, '%%Y-%%m-%%d %%H:%%i:%%s'), %s, %s)
                          """
                    cursor.execute(sql, (self.name, self.logonTime, self.logonTime, self.serviceType, self.contact))
                    client.commit()
                else:               # old user
                    sql = """
                          UPDATE `users` SET `logonTime`=STR_TO_DATE(%s, '%%Y-%%m-%%d %%H:%%i:%%s'), `serviceType`=%s, `contact`=%s WHERE `name`=%s
                          """
                    cursor.execute(sql, (self.logonTime, self.serviceType, self.contact, self.name))
                    client.commit()
        finally:
            client.close()

    def _echo(self):
        client = connect()
        try:
            with client.cursor() as cursor:
                sql = """
                      SELECT * FROM `users` WHERE `name`=%s
                      """
                cursor.execute(sql, (self.name,))
                result = cursor.fetchone()
                print(result)
        finally:
            client.close()


if __name__ == '__main__':
    create_all()

    logonTime = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    serviceType = 'service1'
    contact = 'account1@password1@localhost'
    user = User('test', '18800201234', logonTime, serviceType, contact)
    user.commit()
    user._echo()

    serviceType = 'service2'
    contact = 'account2@password2@localhost'
    user = User('test', '18800201234', logonTime, serviceType, contact)
    user.commit()
    user._echo()

    User.fetch_info(id=1)

