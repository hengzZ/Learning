# tcp/udp transmission #


## tcp ##
A clent program that can request a file from the server program.   
The server responds by sending the whole file.   

Usage:
```
$ ./tcp_server
$ ./tcp_client [host name] ../testfile.txt
```


## udp ##
A UDP server receives message from client and replys with same message   

1. change IP same as host IP (udp_client.c)
```
addr.sin_addr.s_addr = inet_addr("192.168.1.100")
```
2. run server
```
$ ./udp_server
```
3. run client
```
$ ./udp_clent
$ hello
```

