#include "flight.h"

Passenger::Passenger() {
    order_list = new Order();
    order_list->next = nullptr;
}

Passenger::Passenger(string& name, string& pass) {
    username = name;
    password = pass;
    order_list = new Order();
    order_list->next = nullptr;
}

Passenger::~Passenger() {}

void Passenger::InsertOrder(Order& order) {
    auto p = new Order(order);
    p->next = order_list->next;
    order_list->next = p;
}

string Passenger::MyTick() {
    string res;
    for (auto p = order_list->next; p; p = p->next) {
        res += p->flight_num + ' ' + (p->finished ? "1 " : "0 ") +
               std::to_string(p->id) + ' ' + std::to_string(p->grade) + ' ' +
               std::to_string(p->order_num) + '\n';
    }
    return res;
}

void Passenger::Refund(int id) {
    for (auto p = order_list; p->next; p = p->next) {
        if (p->next->id == id) {
            auto temp = p->next->next;
            delete p->next;
            p->next = temp;
        }
    }
}

int Flight::id = 0;

Passenger& Flight::FindUser(string& s) {
    for (auto p = users->next; p; p = p->next) {
        if (p->username == s) {
            return *p;
        }
    }
}

Flight::Flight() {}

Flight::Flight(std::ifstream& in,
               Passenger* p) {  // 字符串以空格分割，依次对应航班信息
    in >> destination >> flight_num >> plane_num >> work_day >> max_people[0] >>
        max_people[1] >> max_people[2] >> now_ticket[0] >> now_ticket[1] >>
        now_ticket[2];

    // 还需要建立订票队列和候补队列
    users = p;
    have_ordered = new Order();
    have_ordered->next = nullptr;
    InitOrderList();
    InitWaitingList();
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
            p->finished = true;
            p->flight_num = flight_num;
            p->id = id++;
            FindUser(p->name).InsertOrder(*p);
        }
    }
}
void Flight::InitWaitingList() {
    std::ifstream in("./resources/" + flight_num + "_W.txt");
    if (in.is_open()) {
        while (!in.eof()) {
            Order x;
            in >> x.name >> x.grade >> x.order_num;
            x.finished = false;
            x.flight_num = flight_num;
            x.id = id++;
            FindUser(x.name).InsertOrder(x);
            wait.enQueue(x);
        }
    }
}

void Flight::save(std::ofstream& out) {
    out << destination << ' ' << flight_num << ' ' << plane_num << ' '
        << work_day << ' ' << max_people[0] << ' ' << max_people[1] << ' '
        << max_people[2] << ' ' << now_ticket[0] << ' ' << now_ticket[1] << ' '
        << now_ticket[2] << '\n';
    SaveOrderList();
    SaveWaitingList();
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
        mQueue<Order> temp = wait;
        while (!temp.isEmpty()) {
            auto front = temp.deQueue();
            out << front.name << ' ' << front.grade << ' ' << front.order_num
                << '\n';
        }
        out.close();
    }
}

void Flight::Book(string& name, int grade, int num, bool force) {}

void Flight::Refund(int id) {
    // 航线已订票的链表中需要删除这一项
    // 检查队列，看看能不能解决一些需求
    // 记得更新航线的数据
}

string& Flight::GetFlight() {
    return flight_num;
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