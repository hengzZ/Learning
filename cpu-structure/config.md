### CPU 配置相关命令/工具

##### 查看 cpu 运行时主频
```bash
$ turbostat -i 1 -c 0-17  # -i 信息采集间隔， -c 指定观测的核心
```

##### cpu 功耗模式设置
```bash
# CPU 功耗模式设置
$ cpupower  frequency-set -g performance
# CPU 功耗模式查看
$ cpupower help frequency-info
$ cpupower frequency-info -p
$ cpupower idle-info
```

##### 查看/设置 cpu C-State
```bash
$ cpupower idle-set
$ cpupower idle-info
```

##### msr 寄存器读写工具安装
```bash
$ yum install msr-tools
```

##### LLC/MEM Bandwidth 控制
```bash
pqos （RDT-Resource Director Technology）
```

##### 系统调用分析
```bash
$ perf top
```

##### cpu 利用率查看
```bash
$ top
$ htop
```

##### uncore 频率设置
```bash
wrmsr -a 0x620  0x1818  # 2.4GHz
```

##### cpu 绑定
```bash
numactl -C 0-35 <command to run workload>
```

##### 其他状态查看
```bash
$ sar -A 1
$ iostat -x 1
$ mpstat 1
```

##### Debian 系列 Linux 内核切换
```
# 查看内核列表
$ dpkg --list | grep linux-image
# 查看 grub 信息
grep menuentry /boot/grub/grub.cfg
# 修改 /etc/default/grub 指定启动时的内核版本
vim /etc/default/grub 修改 GRUB_DEFAULT=0 为 GRUB_DEFAULT=2
注意，推荐使用字符串配置法，如“Ubuntu, with Linux 5.0.0-31-generic”，而不是使用 2。
# 更新 grub 配置
sudo update-grub
# reboot 重启，并使用 uname -a 查看内核版本。
```

##### perf 采集 CPU 运行性能: MIPS、CPI
```
$ perf stat -e "cycles,instructions" -I 1000  # 1000 表示1s 刷新一次
$ perf list  # 查看支持多少事件
$ perf record -e "cycles,instructions" ls  # 表示抓 ls 这个命令得所有得事件，用 perf report 查看结果
```

### 查看 CPU 的某个核上运行的进程信息
``CORENUM=34; ps -e -o pid,psr,cpu,cmd | grep -E  "^[[:space:]][[:digit:]]+[[:space:]]+${CORENUM}"``

``CORENUM`` 为 htop 里面看到的核 id 减去 1。
