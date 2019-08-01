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