每个 order 都有一个 id
基本单位是一个个 order
退票只能退买过的票
每个 user 会记录所有买过的票和正在候补的票，啊，候补完了就给转正就行


获取自己所有的订票记录，需要知道记录的 id 和 是在候补还是转正了
需要能够订票和强制订票（允许候补）
需要能够退票（退指定 id 的票）

查询满足条件票是最简单的，直接GET请求明文写好就行了。

然后需要注册，登录，获取自己记录，订票，强制订票，退票
都是POST，逻辑如下：
state参数表示请求的接口
state==1则请求注册
    需求参数：username,password
    返回值：true/false （用户名被占用）
state==2则请求登录
    需求参数：username,password
    返回值：true/false （是否账户密码匹配）
state==3则获取自己记录
    需求参数：username,password
    返回值：每一行都是订票的信息，包含id，是否完成，以及航班信息
state==4则进行订票
    需求参数：username,password,flight,grade,num
    返回值：无所谓，订完票直接刷新界面
state==5则强制订票
    需求参数：username,password,flight,grade,num
    返回值：同上
state==6则退票
    需求参数：username,password,id
    返回值：同上

此外，POST还需要能够根据 admin 账户的需要，保存订票状态
这里参数admin="password"写死就可以了