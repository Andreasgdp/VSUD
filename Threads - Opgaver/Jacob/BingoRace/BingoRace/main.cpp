#include <iostream>
#include <thread>

void printBingo() {
    std::cout << " raaber \"BINGO\"" << std::endl;
}


int main() {
    std::thread first(printBingo);
    //thread second([](string name){cout << name << " raaber \"BINGO\"" << endl;}, "Claire");

    first.join();
    //second.join();

    return 0;
}
