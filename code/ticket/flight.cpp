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

void Passenger::Book(Order& order) {
    auto p = new Order(order);
    p->next = order_list->next;
    order_list->next = p;
}

void Passenger::Refund(int id) {
    for (auto p = order_list; p->next; p = p->next) {
        if (p->next->id == id) {
            auto temp = p->next->next;
            delete p->next;
            p->next = temp;
            return;
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
    std::ifstream in("./data/" + flight_num + "_O.txt");
    if (in.is_open()) {
        while (in.peek() != EOF) {
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
    std::ifstream in("./data/" + flight_num + "_W.txt");
    if (in.is_open()) {
        while (in.peek() != EOF) {
            Order x;
            in >> x.name >> x.grade >> x.order_num;
            x.finished = false;
            x.flight_num = flight_num;
            x.id = id++;
            FindUser(x.name).InsertOrder(x);
            wait[x.grade].enQueue(x);
        }
    }
}

void Flight::save(std::ofstream& out) {
    out << '\n'
        << destination << ' ' << flight_num << ' ' << plane_num << ' '
        << work_day << ' ' << max_people[0] << ' ' << max_people[1] << ' '
        << max_people[2] << ' ' << now_ticket[0] << ' ' << now_ticket[1] << ' '
        << now_ticket[2];
    SaveOrderList();
    SaveWaitingList();
}

void Flight::SaveOrderList() {
    std::ofstream out("./data/" + flight_num + "_O.txt");
    if (out.is_open()) {
        for (auto p = have_ordered->next; p; p = p->next) {
            out << '\n' << p->name << ' ' << p->grade << ' ' << p->order_num;
        }
        out.close();
    }
}

void Flight::SaveWaitingList() {
    std::ofstream out("./data/" + flight_num + "_W.txt");
    if (out.is_open()) {
        for (int i = 0; i < 3; i++) {
            mQueue<Order> temp = wait[i];
            while (!temp.isEmpty()) {
                auto front = temp.deQueue();
                out << '\n'
                    << front.name << ' ' << front.grade << ' '
                    << front.order_num;
            }
        }

        out.close();
    }
}

bool Flight::Verify(int grade, int num) {
    return now_ticket[grade] >= num ? true : false;
}

bool Flight::Book(string& name, int grade, int num, bool force) {
    if (max_people[grade] < num ||
        (now_ticket[grade] < num && !force))  // 不够卖
        return false;
    auto p = new Order();
    p->name = name;
    p->flight_num = flight_num;
    p->id = id++;
    p->grade = grade;
    p->order_num = num;
    if (now_ticket[grade] >= num) {  // 余票还够
        p->finished = true;
        FindUser(name).Book(*p);
        now_ticket[grade] -= num;
        p->next = have_ordered->next;
        have_ordered->next = p;
    } else {
        p->finished = false;
        FindUser(name).Book(*p);
        wait[grade].enQueue(*p);
        delete p;
    }
    return true;
}

void Flight::Refund(int id) {
    // 航线已订票的链表中需要删除这一项
    int grade = 0;
    for (auto p = have_ordered; p->next; p = p->next) {
        if (p->next->id == id) {
            grade = p->next->grade;
            now_ticket[p->next->grade] += p->next->order_num;
            auto temp = p->next->next;
            delete p->next;
            p->next = temp;
            break;
        }
    }
    // 检查队列，看看能不能解决一些需求
    while (!wait[grade].isEmpty()) {
        auto order = new Order(wait[grade].getFront());
        if (now_ticket[order->grade] >= order->order_num) {
            // 一方面是直接以现在的 id 插入到订票列表里
            now_ticket[order->grade] -= order->order_num;
            order->next = have_ordered->next;
            have_ordered->next = order;
            // 一方面是要把用户链表里的 finished 状态改掉
            for (auto p = FindUser(order->name).order_list->next; p;
                 p = p->next)
                if (p->id == order->id) {
                    p->finished = true;
                    break;
                }
            wait[grade].deQueue();
        } else {
            break;
        }
    }
}

string& Flight::GetFlight() {
    return flight_num;
}

string& Flight::GetDestination() {
    return destination;
}

string Flight::show() {
    return destination + ' ' + flight_num + ' ' + plane_num + ' ' +
           std::to_string(work_day) + ' ' + std::to_string(max_people[0]) +
           ' ' + std::to_string(max_people[1]) + ' ' +
           std::to_string(max_people[2]) + ' ' + std::to_string(now_ticket[0]) +
           ' ' + std::to_string(now_ticket[1]) + ' ' +
           std::to_string(now_ticket[2]) + '\n';
}