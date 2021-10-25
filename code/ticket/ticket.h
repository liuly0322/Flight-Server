#ifndef TICKET
#define TICKET

#include "flight.h"
class Ticket {
   private:
    Passenger* users;  // 用户对应的链表
    Flight* flights;   // 航班的顺序表
    int flights_num;   // 一共有多少个航班

   public:
    Ticket();
    ~Ticket();

    void query();
};

#endif  // TICKET