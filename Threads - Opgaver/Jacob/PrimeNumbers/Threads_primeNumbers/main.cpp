#include <thread>
#include <pthread.h>
#include <chrono>
#include <iostream>
#include <random>
#include <mutex>
#include <algorithm>

using namespace std;

int primes[] = {1,2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};

mutex arr_mutex;

bool isPrime(int num){
    return (find(begin(primes), end(primes), num) != end(primes));
}

bool carryon(int arr[], int arrSize) {
    // Are all prime numbers in the array, return false, else true
    int primesSize = sizeof(primes)/sizeof(primes[0]);
    int j = 0;
    for (int i = 0; i < primesSize; i++) {
        for (j = 0; j < arrSize; j++) {
            if (primes[i] == arr[j]) {
                break;
            }
        }
        if (j == arrSize) {
            return true;
        }
    }
    return false;
}

void insertNum(int arr[], int arrSize) {
    std::uniform_int_distribution<int> distribution(1, 50);
    std::random_device rd;
    std::default_random_engine generator(rd());

    for (int i = 0; i < arrSize; i++) {
        if (arr[i] == 0) {
            arr[i] = distribution(generator);
            break;
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void removeNums(int arr[], int arrSize) {
    for (int i = 0; i < arrSize; i++) {
        if (!isPrime(arr[i])) {
            arr[i] = 0;
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int usedElems(int arr[], int arrSize) {
    int count = 0;
    for (int i = 0; i < arrSize; i++) {
        if (arr[i] != 0) {
            count += 1;
        }
    }
    return count;
}

int main()
{
    int arr[100] = {0};
    int arrSize = sizeof(arr)/sizeof(arr[0]);

    thread producer{insertNum, arr, arrSize};
    thread consumer{removeNums, arr, arrSize};

    producer.join();
    consumer.join();

    cout << "size: " << usedElems(arr, arrSize) << endl;


    //cout << "notPrime: " << isPrime(4) << endl;

    /*
    int arr[] = {1,2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,4};
    int arrSize = sizeof(arr)/sizeof(arr[0]);

    //insertNum(arr, arrSize);

    cout << "arr: " << endl;
    for (size_t i = 0; i < arrSize; i++) {
        std::cout << arr[i] << ' ';
    }
    cout << endl;

    removeNums(arr, arrSize);

    cout << "arr: " << endl;
    for (size_t i = 0; i < arrSize; i++) {
        std::cout << arr[i] << ' ';
    }
    cout << endl;
    */

    return 0;
}
