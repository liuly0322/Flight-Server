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

   public:
    Ticket();
    ~Ticket();

    bool Login(string& user, string& pass);  // 返回是否登陆成功
    bool Regi(string& user, string& pass);  // 失败可能是用户名占用等

    void query();
};

#endif  // TICKET