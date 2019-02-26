## nohup 后台运行


#### 1. 只输出错误信息到日志文件
```
nohup ./program >/dev/null 2>log &
```


#### 2. 不生成日志文件
```
nohup ./program >/dev/null 2>&1 &
```
