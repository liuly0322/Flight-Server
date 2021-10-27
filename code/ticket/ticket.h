#ifndef TICKET
#define TICKET

#include <fstream>
#include <iostream>

#include "flight.h"
class Ticket {
   private:
    Passenger* users;  // 用户对应的链表
    Flight* flights;   // 航班的顺序表

    void UsersInit();
    void FlightsInit();

    void UsersSave();
    void FlightsSave();

   public:
    Ticket();
    ~Ticket();

    void Save();  // 将当前状态写入到文件里

    bool Login(string& name, string& pass);  // 返回是否登陆成功
    bool Regi(string& name, string& pass);  // 失败可能是用户名占用等

    // 订票
    void Book(string& name, string& flight, int grade, int num, bool force);
    // 退票
    void Refund(string& name, int id);

    // 查询该用户当前订票情况
    string MyTick(string& name);

    // 查询城市或航班号所有线路
    string query(string& s);
};

#endif  // TICKET