#include <thread>
#include <chrono>
#include <iostream>
#include <random>
#include <mutex>
#include <algorithm>

using namespace std;

int primes[] = {1,2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};

mutex arr_mutex;

bool notPrime(int num){
    return !(find(begin(primes), end(primes), num) != end(primes));
}

bool isSubset(int arr1[], int arr2[], int m, int n) {
    int i = 0;
    int j = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            if(arr2[i] == arr1[j])
                break;
        }

        if (j == m)
            return false;
    }
    return true;
}


bool carryon(int arr[]) {
    return isSubset(arr, primes, sizeof(&arr)/sizeof(arr[0]), sizeof(&primes)/sizeof(primes[0]));
}

void insertNum(int arr[]) {
    /*
     * Produceren genererer tilfældige heltal mellem 1 og 50, som placeres i en tabel/collection.
     * Hver gang den har indsat et nyt tal sover den i 100 millisekunder.
    */

}

void removeNums(int arr[]) {

}


int main()
{
    int arr[100] = {0};
    thread producer{insertNum, arr};
    thread consumer{removeNums, arr};

    producer.join();
    consumer.join();

    cout << "sze: " << sze << endl;
    return 0;
}
