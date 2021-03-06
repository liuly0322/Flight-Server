#include "ticket.h"

using std::cin;
using std::cout;
using std::fstream;
using std::ifstream;
using std::ofstream;

void Ticket::UsersInit() {
    // 初始用户数据从 users.txt 中读取
    // 约定每一行空格分割 用户名 和 密码
    ifstream in("./data/users.txt");
    if (in.is_open()) {
        while (in.peek() != EOF) {
            // 头插法插入链表
            Passenger* p = new Passenger();
            in >> p->username >> p->password;
            p->next = users->next;
            users->next = p;
        }
    }
}

void Ticket::FlightsInit() {
    ifstream in("./data/flights.txt");
    if (in.is_open()) {
        while (in.peek() != EOF) {
            Flight* p = new Flight(in, users);
            p->next = flights->next;
            flights->next = p;
        }
    }
}

Ticket::Ticket() {
    // 初始的参数由文件提供
    users = new Passenger();
    users->next = nullptr;

    flights = new Flight();
    flights->next = nullptr;

    UsersInit();
    FlightsInit();
}

Ticket::~Ticket() {}

bool Ticket::Login(string& name, string& pass) {
    // 需要遍历 users 链表
    for (auto p = users->next; p; p = p->next) {
        if (p->username == name && p->password == pass) {
            return true;
        }
    }
    return false;
}

void Ticket::Save() {  // 保存
    // 分别需要保存 users.txt, flights.txt和调用flight函数保存每个flight信息
    UsersSave();
    FlightsSave();
}

void Ticket::UsersSave() {
    ofstream out("./data/users.txt");
    if (out.is_open()) {
        for (auto p = users->next; p; p = p->next) {
            out << '\n' << p->username << ' ' << p->password;
        }
        out.close();
    }
}

void Ticket::FlightsSave() {
    ofstream out("./data/flights.txt");
    if (out.is_open()) {
        for (auto p = flights->next; p; p = p->next) {
            p->save(out);
        }
        out.close();
    }
}

bool Ticket::Regi(string& name, string& pass) {
    // 需要遍历 users 链表，判断用户名是否重复
    for (auto p = users->next; p; p = p->next) {
        if (p->username == name) {
            return false;
        }
    }
    // 接下来需要将该用户存入用户链表中
    auto p = new Passenger(name, pass);
    p->next = users->next;
    users->next = p;
    return true;
}

bool Ticket::Book(string& name,
                  string& flight,
                  int grade,
                  int num,
                  bool force) {
    for (auto p = flights->next; p; p = p->next) {
        if (p->GetFlight() == flight) {
            return p->Book(name, grade, num, force);
        }
    }
    return false;
}

void Ticket::Refund(string& name, int id) {
    for (auto user = users->next; user; user = user->next) {
        if (user->username == name) {
            // 找到用户了，下面开始找 id
            for (auto order = user->order_list->next; order;
                 order = order->next) {
                if (order->id == id) {
                    // 找到 id 了，再找到对应的航班去退票
                    for (auto flight = flights->next; flight;
                         flight = flight->next) {
                        if (flight->GetFlight() == order->flight_num) {
                            // 找到航班了，去退票
                            user->Refund(id);
                            flight->Refund(id);
                            return;
                        }
                    }
                }
            }
        }
    }
}

string Ticket::MyTick(string& name) {
    for (auto p = users->next; p; p = p->next) {
        if (p->username == name) {
            // 这就是需要找的用户
            return p->MyTick();
        }
    }
    return "";
}

string Ticket::query(string& s) {
    // 输入城市或航班，查询所有符合要求的航班
    string res;
    for (auto p = flights->next; p; p = p->next) {
        if (p->GetDestination() == s || p->GetFlight() == s) {
            res += p->show();
        }
    }
    return res;
}

string Ticket::query(string& s, int grade, int num) {
    // 查询还需要验证余票是否符合
    string res;
    for (auto p = flights->next; p; p = p->next) {
        if ((p->GetDestination() == s || p->GetFlight() == s) &&
            p->Verify(grade, num)) {
            res += p->show();
        }
    }
    return res;
}

string Ticket::AdminQuery(string& s) {
    // 找到指定的航班
    for (auto p = flights->next; p; p = p->next) {
        if (p->GetFlight() == s) {
            return p->ShowBook();
        }
    }
    return "";
}
