啊，很整蛊

相当于是每个 order 都有一个 id
基本单位是一个个 order
退票只能退买过的票，啊，不可以退候补队列中的票
每个 user 会记录所有买过的票和正在候补的票，啊，候补完了就给转正就行

接口汇总一下：

注册，登录；

查询指定城市的所有路线

获取自己所有的订票记录，啊，需要知道记录的 id 和 是在候补还是转正了
啊，需要能够订票和强制订票（允许候补）
啊，需要能够退票（退指定 id 的票）

查询城市的票是最简单的，直接GET请求明文写好就行了。

然后需要注册，登录，获取自己记录，订票，强制订票，退票
都是POST，逻辑如下：
state参数表示请求的接口
state=="1"则请求注册
    需求参数：username,password
    返回值：true/false （用户名被占用）
state=="2"则请求登录
    需求参数：username,password
    返回值：true/false （是否账户密码匹配）
state=="3"则获取自己记录
    需求参数：username,password
    返回值：每一行都是订票的信息，包含id，是否完成，以及航班信息
state=="4"则进行订票
    需求参数：username,password,flight,grade,num
    返回值：是否成功（其实无所谓，订完票直接刷新界面）
state=="5"则强制订票
    需求参数：username,password,flight,grade,num
    返回值：同上
state=="6"则退票
    需求参数：username,password,flight
    返回值：同上

此外，POST还需要能够根据 admin 账户的需要，保存订票状态
这里参数admin="password"写死就可以了