#include <iostream>
#include <thread>
#include <random>
#include <mutex>
#include <chrono>
#include <QSqlDatabase>
#include <QtSql>
#include <QCoreApplication>
#include <QSqlDriver>

#include "NumberPlateGenerator.h"
#include "Database.h"

using namespace std;

mutex mtx;

string modeller[] = {"Audi","BMW","VW","Mercedes","Volvo"};
string ejerer[] = {"Jens","Peter","Knud","Hans","Egon"};

NumberPlateGenerator NumberPlateGenerator;
Database db;

std::uniform_int_distribution<int> distribution(0, 4);
std::uniform_int_distribution<int> aargang(1900, 2021);
std::random_device rd;
std::default_random_engine generator(rd());

void generateData(char* numberPlate) {
    for (int i = 0; i < 20000; i++) {
        NumberPlateGenerator.generate(numberPlate);
        db.insertCar(numberPlate, modeller[distribution(generator)], aargang(generator), ejerer[distribution(generator)]);
    }
}

bool carryon() {
    return true;
}

// Ved køb af bil
void addCar(char* numberPlate) {
    NumberPlateGenerator.generate(numberPlate);
    cout << numberPlate << endl;
    db.insertCar(numberPlate, modeller[distribution(generator)], aargang(generator), ejerer[distribution(generator)]);
    while (carryon()) {
        //mtx.lock();
//        NumberPlateGenerator.generate(numberPlate);
//        db.insertCar(numberPlate, modeller[distribution(generator)], aargang(generator), ejerer[distribution(generator)]);
        //mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

// Ved skrotning af bil
void removeCar() {
    while (carryon()) {
        mtx.lock();

        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
}

// Ved ejer skifte af bil
void reRegisterCar(char* numberPlate) {
    while (carryon()) {
        mtx.lock();

        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}


int main()
{
    db.createDatabase();

    char* numberPlate = new char[7];
    if (db.isEmpty())
        generateData(numberPlate);

    thread adder{addCar, numberPlate};
    thread deleter{removeCar};
    thread reRegister{reRegisterCar, numberPlate};

    adder.join();
    deleter.join();
    reRegister.join();

    return 0;
}
