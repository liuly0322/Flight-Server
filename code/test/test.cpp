#include <fstream>
#include <iostream>
#include <string>
using std::string;
template <class T>
struct QNode {
    T data;
    struct QNode<T>* next;
};

template <class T>
class mQueue {
   private:
    struct QNode<T>* front;  // 队头
    struct QNode<T>* rear;   // 队尾
    // 队头是一个空结点
    // 队尾指向最后一个元素结点

    int size;  // 队长

   public:
    mQueue();
    ~mQueue();
    void enQueue(T& x);  // 入队
    T deQueue();         // 出队
    T getFront();        // 获得队头元素
    bool isEmpty();      // 判空
    int length();        // 返回长度
};

template <class T>
mQueue<T>::mQueue() {
    front = new QNode<T>();
    rear = front;
    size = 0;
    front->next = nullptr;
}

template <class T>
mQueue<T>::~mQueue() {
    while (front) {
        // rear 用于暂时存储队头的下一个结点
        rear = front->next;
        delete front;
        front = rear;
    }
}

template <class T>
void mQueue<T>::enQueue(T& x) {
    struct QNode<T>* p = new QNode<T>();
    p->data = x;
    p->next = nullptr;
    rear->next = p;
    rear = p;
    size++;
}

template <class T>
T mQueue<T>::deQueue() {  // 删除队头元素
    struct QNode<T>* p = front->next->next;
    T ret = front->next->data;
    delete front->next;
    front->next = p;
    size--;
    return ret;
}

template <class T>
T mQueue<T>::getFront() {  // 获得队头元素
    return front->next->data;
}

template <class T>
bool mQueue<T>::isEmpty() {
    return size == 0 ? true : false;
}

template <class T>
int mQueue<T>::length() {
    return size;
}

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

    void query();
};

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
    InitOrderList(flight_num + "_O.txt");

    InitWaitingList(flight_num + "_W.txt");
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
using std::cin;
using std::cout;
using std::fstream;
using std::ifstream;
using std::ofstream;

void Ticket::UsersInit() {
    // 初始用户数据从 users.txt 中读取
    // 约定每一行空格分割 用户名 和 密码
    ifstream users_in("users.txt");
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
    ifstream flights_in("flights.txt");
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

using namespace std;
int main() {
    Ticket tick;
    string username = "xiaoming", password = "fi1ne";
    if (tick.Login(username, password)) {
        cout << "Login!";
    }
}