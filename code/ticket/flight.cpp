#include "flight.h"

Flight::Flight() {}

Flight::Flight(std::ifstream& in) {  // 字符串以空格分割，依次对应航班信息
    in >> destination >> flight_num >> plane_num >> work_day >> max_people[0] >>
        max_people[1] >> max_people[2] >> now_ticket[0] >> now_ticket[1] >>
        now_ticket[2];
    // 还需要建立订票队列和候补队列
    // 这里不妨认为，假设航班号是 "HF0101"，则订票链表对应的文件为
    // "HF0101_O.txt" 相应地，候补队列对应的文件为 "HF0101_W.txt"

    // 需要先初始化链表
    have_ordered = new Order();
    have_ordered->next = nullptr;
    InitOrderList("./resources/" + flight_num + "_O.txt");

    InitWaitingList("./resources/" + flight_num + "_W.txt");
}

Flight::~Flight() {
    // 析构函数，需要做的事情是销毁链表
}

void Flight::InitOrderList(string s) {
    // 传递进来文件名
    std::ifstream in(s);
    if (in.is_open()) {
        while (!in.eof()) {
            Order* p = new Order();
            in >> p->name >> p->grade >> p->order_num;
            p->next = have_ordered->next;
            have_ordered->next = p;
        }
    }
}
void Flight::InitWaitingList(string s) {
    // 是一个队列，每次从队尾插入即可
    // 传递进来文件名
    std::ifstream in(s);
    if (in.is_open()) {
        while (!in.eof()) {
            Order x;
            in >> x.name >> x.grade >> x.order_num;
            wait.enQueue(x);
        }
    }
}