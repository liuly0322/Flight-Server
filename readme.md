# Flight Web Server

* 基于 [TinyWebServer](https://github.com/qinguoyi/TinyWebServer) 项目

## 环境要求
* Linux
* C++14
* MySql

## 功能说明

本次实验除了完成基础部分之外，另外完成的功能有：

- 图形化界面

- Http 通信

  - 主体基于开源项目 WebServer 修改 HttpRequest 和 HttpConn 类而来，修改后支持了对 Get 请求的解析以及返回字符串而非文件

  - 互斥锁，同一时间只有一个线程在访问 ticket 类

- 简易账户密码系统

- 管理员可以查看航班的所有订票用户

- 数据可以保存在文件中

- 支持查询到终点站的所有线路

- 订票

  - 支持一个用户同一航线可以多次订票

  - 支持在用户订票需求不能满足时返回所有满足需求的同目的地航线

## 接口约定

如果传入 s 则表示查询城市 / 航线

此时，可能有 grade 和 num 参数来表示是否要求满足某一等舱的余票数不少于指定值

否则：

- state == 1 则请求注册
    需求参数：username,password
    返回值：true/false （用户名被占用）

- state == 2 则请求登录
    需求参数：username,password
    返回值：true/false （是否账户密码匹配）

- state == 3 则获取自己记录
    需求参数：username,password
    返回值：每一行都是订票的信息，包含id，是否完成，以及航班信息

- state == 4 则进行订票
    需求参数：username,password,flight,grade,num
    返回值：成功或失败

- state == 5 则强制订票
    需求参数：username,password,flight,grade,num
    返回值：无所谓，订完票直接刷新界面

- state == 6 则退票
    需求参数：username,password,id
    返回值：同上

此外，POST还需要能够根据 admin 账户的需要，保存订票状态，这里参数admin="password"写死就可以了

## 目录树
```
.
├── bin             // 程序
│   ├── log
│   └── server
├── build           // 编译
│   └── Makefile
├── code            // 后端代码
│   ├── buffer
│   ├── config
│   ├── http        // http 连接
│   ├── log
│   ├── main.cpp
│   ├── pool
│   ├── readme.md
│   ├── server
│   ├── ticket      // 订票系统
│   └── timer
├── front-end       // 前端界面
│   ├── axios.min.js
│   ├── index.html
│   ├── logo.png
│   ├── main.css
│   ├── main.js
│   └── pinyin.js
├── LICENSE
├── Makefile
├── readme.md
└── resources       // 静态文件
```


## 项目启动

```bash
make
./bin/server
```

前端都是静态资源，自己挂载上即可
