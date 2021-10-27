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

    bool Login(string& user, string& pass);  // 返回是否登陆成功
    bool Regi(string& user, string& pass);  // 失败可能是用户名占用等

    // 订票，不候补
    bool Order(string& user, string& flight, int grade, int num);
    // 订票，支持候补
    bool OrderF(string& user, string& flight, int grade, int num);

    // 查询该用户当前订票情况
    string MyTick(string& user);

    string query(string& city);
};

#endif  // TICKET