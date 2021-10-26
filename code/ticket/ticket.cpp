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
            Flight* p = new Flight(flights_in);
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

Ticket::~Ticket() {
    // 析构，需要将一切写回到文件里
}

bool Ticket::Login(string& user, string& pass) {
    // 需要遍历 users 链表
    for (auto p = users->next; p; p = p->next) {
        if (p->username == user && p->password == pass) {
            return true;
        }
    }
    return false;
}

void Ticket::query() {}