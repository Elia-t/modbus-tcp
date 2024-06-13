### description
``` txt
基于libmodbus编写的自动重连的modbus-TCP主站。代码水准很低，代码结构也很混乱，请多见谅。
一次读/写操作连续失败的次数超过设定值则认为与从站连接断开。
非常粗暴的方式，也许有更好的方式判断与从站的连接断开。
```
A modbus-TCP master with automatic reconnection based on libmodbus. The code level is very low and the code structure is confusing, please forgive me.
A read/write operation that fails consecutively more than a set number of times is considered to be disconnected from the slave.
Very crude way, maybe there is a better way to judge the connection with the slave is disconnected.

### dependencies
[libmodbus](https://github.com/stephane/libmodbus) and [Documentation](https://libmodbus.org/reference)
``` shell
sudo apt-get install libmodbus-dev
```

