## Linux Common Command

##### 1. Update Linux Headers
```
sudo apt-get update
sudo apt-get install linux-source
sudo apt-get install linux-headers-$(uname -r)
```

##### 2. 修改文件所属用户组/所有者
```
# 修改文件 install.log 的所属用户组为 users
$ chgrp users -R install.log  
# 修改 install.log 的所有者为 bin
$ chown bin -R install.log
```

##### 3. Linux Operation System Clone (DD Tool)
```
# Clone the Image into USB media
$ zcat centos7.0_deverton.img.gz | dd of=/dev/sdc
# 查看 clone 进度
$ sudo watch -n 5 killall -USR1 dd

reference:
https://www.cnblogs.com/answercard/p/6213134.html
http://man.linuxde.net/dd
```

##### 4. 开机启动分区自动挂载
```
# 编辑 /etc/fstab　文件
# 添加： /dev/sda1 /test ext3 defaults 1 1
```

##### 5. Linux 磁盘分区
```
# su 进入超级用户
# 分区操作：
    fdisk /dev/sda
    Command (m for help): n
    Command action
    e extended
    p primary partition (1-4)
    输入：e
    Partition number (1-4): 1
    First cylinder (1-9729, default 1):回车
    Last cylinder or +size or +sizeM or +sizeK (1-9729, default 9729):回车
    Command (m for help):w(保存退出)
```

##### 6. 磁盘挂载操作：
```
$ fdisk -l
$ mkfs -t ext3 /dev/sda1
# Writing superblocks and filesystem accounting information: 直接回车
$ mount /dev/sda1 /test
```

##### 7. 添加用户
```
$ useradd zhiheng
$ passwd zhiheng
```

##### 8. Linux 命令后面加 & (后台运行)
```
$ numactl -C 0-25,104-129 -l ./src/sysbench --num-threads=$threads --test=./tests/include/oltp_legacy/oltp.lua --oltp-table-size=2180000  --oltp-tables-count=10 --oltp-point-selects=1 --oltp-simple-ranges=0 --oltp-sum-ranges=0 --oltp-order-ranges=0 --oltp-distinct-ranges=0 --oltp-skip-trx=on --oltp-read-only=on --percentile=99  --report-interval=10  --mysql-host=192.168.14.150 --mysql-port=8002 --mysql-user=inteltest --mysql-password=inteltest --mysql-db=sbtest1 --max-time=300 --max-requests=0  --oltp_auto_inc=off run >log/sysbench_ps_8002_1.log 2>&1 &
```

##### 9. 返回至上一次的工作目录
```
$ cd -
```
