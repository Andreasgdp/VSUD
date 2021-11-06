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

void addCar(char* numberPlate) {

}

void removeCar() {

}

void reRegisterCar(char* numberPlate) {

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
