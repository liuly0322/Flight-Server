#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream in("users.txt");
    // 来测试空文件
    int a, i = 1;
    if (in.is_open()) {
        while (in.peek() != EOF) {
            in >> a;
            std::cout << "这是第" << i << "次读入到的数字" << a << '\n';
            i++;
        }
    }
}