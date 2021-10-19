#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;
std::string final_sentence = "";

void go() {
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_one();
}

void printLeaderLetter (std::string letter) {
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);
    std::cout << "Give me a \"" << letter << "\"" << '\n';
    go();
}

void printLetter (std::string letter) {
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);
    std::cout << letter << "!" << '\n';
    final_sentence.append(letter);
}

int main ()
{
    std::string letters[8] = {"G", "o", " ", "T", "e", "a", "m", "!"};
    std::thread threads[8];

    for (int i = 0; i < 8; ++i) {
        threads[i] = std::thread(printLetter, letters[i]);
    }

    go();

    for (int i = 0; i < 8; ++i) {
        printLeaderLetter(letters[i]);
    }

    for (auto& th : threads) th.join();

    std::cout << "What does that say?: " << final_sentence << std::endl;

    return 0;
}
