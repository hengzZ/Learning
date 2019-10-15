## 邮箱服务器

##### 465 端口
###### 被禁用了的 25 端口
阿里云：
  由于国际与国内均对垃圾邮件进行严格管控，另外，鉴于服务器 25 端口被大量垃圾邮件充斥，阿里云服务器不再提供 25 端口邮件服务。

##### 网络中常用的端口号有哪些
* HTTP 80
* FTP 20/21
* Telnet 23
* SMTP 25
* DNS 53
* SMTPS 465
* MSA 587
* 等等

#### 1. SMTPS
全称 SMTP-over-SSL。 SMTPS 和 SMTP 协议一样，也是用来发送邮件的，只是更安全些，防止邮件被黑客截取泄露。

##### SMTP
“Simple Mail Transfer Protocol” 简单邮件传输协议。 SMTP 协议属于 TCP/IP 协议簇，帮助每台计算机在发送或中转信件时找到下一个目的地。

##### SMTP 服务器
SMTP 服务器地址，实际上就是代收发服务器地址，是由邮箱服务商提供的。 例如：
```
1、QQ邮箱（mail.qq.com）
POP3服务器地址：pop.qq.com（端口：110）
SMTP服务器地址：smtp.qq.com（端口：25）
2、搜狐邮箱（sohu.com）:
POP3服务器地址:pop3.sohu.com（端口：110）
SMTP服务器地址:smtp.sohu.com（端口：25）
3、HotMail邮箱（hotmail.com）：
POP3服务器地址：pop.live.com（端口：995）
SMTP服务器地址：smtp.live.com（端口：587）
4、移动139邮箱:
POP3服务器地址：POP.139.com（端口：110）
SMTP服务器地址：SMTP.139.com(端口：25)
5、景安网络邮箱：
POP3服务器地址：POP.zzidc.com（端口：110）
SMTP服务器地址：SMTP.zzidc.com(端口：25)
## 备注： POP 是接收服务器， SMTP 是发送服务器。
```

#### 2. SMTP 与 SMTPS 端口测试
##### 2.1 SMTP 25 连接测试
* 安装 telnet
* 为什么需要 telnet？
  ```
  telnet 可以测试某个端口是否可访问。 telnet 是 Internet 远程登陆服务的标准协议和主要方式。
  ```
* 测试
  ```bash
  telnet smtp.163.com 25
  ```
如果一直在尝试连接的状态，则说明 25 端口是不可用的。

##### 2.2 Telnet 测试 SMTP 收发邮件
##### 背景介绍
```
尽管如今是 WWW 和 IM(instant messaging) 流行的时代，email 这种古老的网路服务依旧是重要的通信工具。
为了要发出一封邮件，Mail Client 会和远端的 Mail Server 建立起通信的管道，两者的通讯要遵守一套规则：
SMTP 协议 （Simple Mail Transfer Protocol）。

其实，可以利用 telnet 工具来扮演 Mail Client 的角色，与 Mail Server 建立联系，并发送邮件。
如何 使用 telnet 来与 Mail Server 建立联系并通信就需要了解 SMTP 协议。
```

##### SMTP 连接与交互
* 连接 SMTP 服务器
  ```bash
  telnet smtp.163.com 25
  ```
* 向服务器打招呼（必须，协议要求），开启与 Mail Server 的对话。
  ```bash
  helo
  ```
* 登陆（邮箱、密码）
  ```bash
  auth login
  ```
  ```bash
  输入邮箱的 BASE64 编码
  ```
  ```bash
  输入密码的 BASE64 编码
  ```
* 告诉 Mail Server 发信者是谁。 （用于邮件发送失败时信件的退回）
  ```bash
  mail from: xxx@xxx.com
  ```
  ```
  # 注意： 该地址不会显示在信件的寄件者栏中。
  # 寄件者欄、收件者欄、標題欄、副本、密件副本欄，這些是在 Message 的 Header 中指定的。
  ```
* 告诉 Mail Server 收件人是谁。
  ```bash
  rcpt to: xxx@xxx.com
  ```
  ```
  # 注意： 如果需要同時寄一封信給很多人，多下幾次 RCPT TO 指令即可。
  ```
* 开始输入邮件正文。
  ```bash
  data
  ```
  ```
  # 输入内容
  # 例如：
  subject: TESTING SMTP
  This is line one.
  This is line two.
  . 
  ```
  ```
  # 注意： Mail Message 包括 Message Header 和 Message Body。
  # 要结束 Message 的输入时，在信件结尾，新的一行单独输入 “.” 符号即可。
  # NOOP 表示 No Operation，用于请求 Server 回应一下，Client 以确认 Sever 的连接还在。 
  ```
* 结束对话。
  ```bash
  quit
  ```

备注： SMTP 交互指令没有大小写的区别。 另外， Mail Server 的 SMTP 交互的返回码对应的意义，请自行查询。

###### 补充： SMTP 常常不可用。。。
```
由于 SMTP 协议极为简单，任何人只要懂得 SMTP 指令，就可以写出自动发送广告、垃圾邮件的程序。
```

##### 2.3 测试 SMTPS 收发邮件
##### 预环境
在 25 端口不可用的情况下，就只能使用 465 端口下的 SMTP-over-SSL 协议来完成。
* 首先，需要获得邮箱的 SSL 证书并存放到本地。
  ``` bash
  # 例如：
  mkdir -p /opt/.certs/
  echo -n | openssl s_client -connect smtp.qq.com:465 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > /opt/.certs/qq.crt
  certutil -A -n "GeoTrust SSL CA" -t "C,," -d  /opt/.certs -i /opt/.certs/qq.crt
  certutil -A -n "GeoTrust Global CA" -t "C,," -d /opt/.certs -i /opt/.certs/qq.crt
  certutil -L -d /opt/.certs
  chmod -R 777 /opt/.certs   #让所有用户都可以发邮件
  # 为了防止发送邮件警告提示，进入目录 /opt/.certs 里执行如下命令：
  certutil -A -n "GeoTrust SSL CA - G3" -t "Pu,Pu,Pu" -d ./ -i qq.crt
  ```
* 然后，选择一个 SMTPS 邮件发送工具。 例如： mailx。
  ```bash
  yum -y install mailx
  ```
  ```bash
  # 配置 mailx， 编辑 /etc/mail.rc 文件进行配置。 在文件最后添加如下信息：
  set from=xxxxxx@qq.com
  set smtp=smtps://smtp.qq.com:465
  set smtp-auth-user=xxxxxx@qq.com
  set smtp-auth-password=你的QQ邮箱授权码
  set smtp-auth=login
  set ssl-verify=ignore
  set nss-config-dir=/opt/.certs  # SSL 证书地址
  ```
* 测试 mailx。
  ```bash
  mailx -s "邮箱测试" xxxx@qq.com < message_file.txt
  ```

##### Python3 实现 163 邮箱 SMTP 发送邮件
使用 SMTP 协议发送邮件，首先需要登陆 163 邮箱查看，你的发件人邮箱是否有开启 SMTP 协议。 在设置中开启 SMTP 协议。

注意： 开通后会让你设置密码，该密码要记好，在进行 SMTP 交互的时候，提到的 password 就是这个授权密码，而不是邮箱密码！！

代码
```python
import smtplib
from email.header import Header
from email.mime.text import MIMEText
 
# 第三方 SMTP 服务
mail_host = "smtp.163.com"      # SMTP 服务器
mail_user = "***"               # 用户名
mail_pass = "***"               # 授权密码，非登录密码
 
sender = 'xxx@163.com'          # 发件人邮箱
receivers = ['xxx@qq.com']      # 接收邮件列表，填几个就是几个收件人。
 
content = '我用Python'
subject = '人生苦短'               # 邮件主题
 
def sendEmail():
    message = MIMEText(content, 'plain', 'utf-8')  # 内容, 格式, 编码
    message['From'] = "{}".format(sender)
    message['To'] = ",".join(receivers)
    message['Subject'] = subject
 
    try:
        smtpObj = smtplib.SMTP_SSL(mail_host, 465)  # 启用 SSL 发信, 端口一般是 465
        smtpObj.login(mail_user, mail_pass)         # 登录验证
        smtpObj.sendmail(sender, receivers, message.as_string())  # 发送
        print("mail has been send successfully.")
    except smtplib.SMTPException as e:
        print(e)
 
def send_email2(SMTP_host, from_account, from_passwd, to_account, subject, content):
    email_client = smtplib.SMTP(SMTP_host)
    email_client.login(from_account, from_passwd)
    # create msg
    msg = MIMEText(content, 'plain', 'utf-8')
    msg['Subject'] = Header(subject, 'utf-8')  # subject
    msg['From'] = from_account
    msg['To'] = to_account
    email_client.sendmail(from_account, to_account, msg.as_string())
    # quit
    email_client.quit()
 
if __name__ == '__main__':
    sendEmail()
    # receiver = 'xxx@xxx.com'
    # send_email2(mail_host, mail_user, mail_pass, receiver, title, content)
```
