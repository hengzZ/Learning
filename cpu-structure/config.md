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
