#ifndef FLIGHT
#define FLIGHT

#include "mqueue.h"

#include <fstream>
#include <iostream>
#include <string>
using std::string;

struct Order {
    string name;
    string flight_num;
    bool finished;  // 是否购入
    int id;         // 订单的 id
    int grade;      // 几等仓(0,1,2)
    int order_num;  // 订票数
    Order* next;    // 链表用
};

class Passenger {
   public:
    string username;    // 名字
    string password;    // 密码
    Passenger* next;    // 链表用
    Order* order_list;  // 用户对应的订票链表

    Passenger();
    Passenger(string& name, string& pass);
    ~Passenger();

    void InsertOrder(Order& order);  // 将订票插入到结点里

    string MyTick();  // 获取本人所有订单情况

    void Book(Order& order);  // 订票
    void Refund(int id);      // 退票
};

class Flight {
   private:
    string destination;     // 目的地
    string flight_num;      // 航班号
    string plane_num;       // 飞机号
    int work_day;           // 星期几（一到七）
    int max_people[3];      // 最大乘坐人数
    int now_ticket[3];      // 余票量
    Order* have_ordered;    // 已订票客户的链表
    mQueue<Order> wait[3];  // 三个舱的候补队列

    Passenger* users;  // 所有用户
    static int id;     // 用于记录 id（全局分配）

    void SaveOrderList();
    void SaveWaitingList();

    void InitOrderList();
    void InitWaitingList();

    Passenger& FindUser(string& s);

   public:
    Flight* next;  // 指向下一个航班

    Flight();
    Flight(std::ifstream& in, Passenger* p);
    ~Flight();

    void save(std::ofstream& out);

    // 订票
    bool Book(string& name, int grade, int num, bool force);

    // 退票
    void Refund(int id);

    // 验证票够不够
    bool Verify(int grade, int num);

    string& GetFlight();
    string& GetDestination();
    string show();
    string ShowBook();
};

#endif