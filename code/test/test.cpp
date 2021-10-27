#include <cstdlib>
#include <iostream>
#include <string>
std::string testfun() {
    std::string res;
    res += "23132";
    res += "is";
    res += "fun";
    return res;
}

int main() {
    std::cout << atoi(testfun().c_str());
}