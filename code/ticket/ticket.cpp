#include "ticket.h"

using std::cin;
using std::cout;
using std::fstream;
using std::ifstream;
using std::ofstream;

void Ticket::UsersInit() {
    // 初始用户数据从 users.txt 中读取
    // 约定每一行空格分割 用户名 和 密码
    ifstream users_in("./resources/users.txt");
    if (users_in.is_open()) {
        while (!users_in.eof()) {
            // 头插法插入链表
            Passenger* p = new Passenger();
            users_in >> p->username >> p->password;
            p->next = users->next;
            users->next = p;
        }
    }
}

void Ticket::FlightsInit() {
    ifstream flights_in("./resources/flights.txt");
    if (flights_in.is_open()) {
        while (!flights_in.eof()) {
            Flight* p = new Flight(flights_in, users);
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

bool Ticket::Login(string& user, string& pass) {
    // 需要遍历 users 链表
    for (auto p = users->next; p; p = p->next) {
        if (p->username == user && p->password == pass) {
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
    ofstream out("./resources/users.txt");
    if (out.is_open()) {
        for (auto p = users->next; p; p = p->next) {
            out << p->username << ' ' << p->password << '\n';
        }
        out.close();
    }
}

void Ticket::FlightsSave() {
    ofstream out("./resources/flights.txt");
    if (out.is_open()) {
        for (auto p = flights->next; p; p = p->next) {
            p->save(out);
        }
        out.close();
    }
}

bool Ticket::Regi(string& user, string& pass) {
    // 需要遍历 users 链表，判断用户名是否重复
    for (auto p = users->next; p; p = p->next) {
        if (p->username == user) {
            return false;
        }
    }
    // 接下来需要将该用户存入用户链表中
    auto p = new Passenger(user, pass);
    p->next = users->next;
    users->next = p;
    return true;
}

string Ticket::query(string& city) {
    // 输入城市，查询所有符合要求的航班
    string res;
    for (auto p = flights->next; p; p = p->next) {
        if (p->GetDestination() == city) {
            res += p->show();
        }
    }
    return res;
}