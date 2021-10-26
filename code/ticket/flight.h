#ifndef FLIGHT
#define FLIGHT

#include "mqueue.h"

#include <fstream>
#include <iostream>
#include <string>
using std::string;

struct Passenger {
    string username;  // 名字
    string password;  // 密码
    Passenger* next;  // 链表用
};

struct Order {
    string name;
    int grade;      // 几等仓(0,1,2)
    int order_num;  // 订票数
    Order* next;    // 链表用
};

class Flight {
   private:
    string destination;   // 目的地
    string flight_num;    // 航班号
    string plane_num;     // 飞机号
    int work_day;         // 星期几（一到七）
    int max_people[3];    // 最大乘坐人数
    int now_ticket[3];    // 余票量
    Order* have_ordered;  // 已订票客户的链表
    mQueue<Order> wait;   // 候补队列

   public:
    Flight* next;  // 指向下一个航班

    Flight();
    Flight(std::ifstream& in);
    ~Flight();

    void InitOrderList(string s);
    void InitWaitingList(string s);
};

#endif