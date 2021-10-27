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
    InitOrderList();

    InitWaitingList();
}

void Flight::save(std::ofstream& out) {
    out << destination << ' ' << flight_num << ' ' << plane_num << ' '
        << work_day << ' ' << max_people[0] << ' ' << max_people[1] << ' '
        << max_people[2] << ' ' << now_ticket[0] << ' ' << now_ticket[1] << ' '
        << now_ticket[2] << '\n';
}

void Flight::SaveOrderList() {
    std::ofstream out("./resources/" + flight_num + "_O.txt");
    if (out.is_open()) {
        for (auto p = have_ordered->next; p; p = p->next) {
            out << p->name << ' ' << p->grade << ' ' << p->order_num << '\n';
        }
        out.close();
    }
}

void Flight::SaveWaitingList() {
    std::ofstream out("./resources/" + flight_num + "_W.txt");
    if (out.is_open()) {
        while (!wait.isEmpty()) {
            auto front = wait.deQueue();
            out << front.name << ' ' << front.grade << ' ' << front.order_num
                << '\n';
        }
        out.close();
    }
}

Flight::~Flight() {}

void Flight::InitOrderList() {
    std::ifstream in("./resources/" + flight_num + "_O.txt");
    if (in.is_open()) {
        while (!in.eof()) {
            Order* p = new Order();
            in >> p->name >> p->grade >> p->order_num;
            p->next = have_ordered->next;
            have_ordered->next = p;
        }
    }
}
void Flight::InitWaitingList() {
    std::ifstream in("./resources/" + flight_num + "_W.txt");
    if (in.is_open()) {
        while (!in.eof()) {
            Order x;
            in >> x.name >> x.grade >> x.order_num;
            wait.enQueue(x);
        }
    }
}

string& Flight::GetDestination() {
    return destination;
}

string Flight::show() {
    return flight_num + ' ' + plane_num + ' ' + std::to_string(work_day) + ' ' +
           std::to_string(max_people[0]) + ' ' + std::to_string(max_people[1]) +
           ' ' + std::to_string(max_people[2]) + ' ' +
           std::to_string(now_ticket[0]) + ' ' + std::to_string(now_ticket[1]) +
           ' ' + std::to_string(now_ticket[2]) + '\n';
}