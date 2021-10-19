#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex talebamse;

class Functor {
public:
    void operator()(std::string name) {
        talebamse.lock();
        std::cout << name << " raaber \"BINGO\"" << std::endl;
        talebamse.unlock();
    }
};

void printBingo(string name) {
    talebamse.lock();
    std::cout << name << " raaber \"BINGO\"" << std::endl;
    talebamse.unlock();
}


int main() {
    thread first(printBingo, "Tom");
    thread second([](string name){talebamse.lock();
                                  cout << name << " raaber \"BINGO\"" << endl;
                                  talebamse.unlock();}, "Claire");
    Functor f;
    thread third(f, "Jason");

    first.join();
    second.join();
    third.join();

    return 0;
}
