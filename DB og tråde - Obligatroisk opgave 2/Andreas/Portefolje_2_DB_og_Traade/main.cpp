#include <iostream>
#include <random>

#include <mutex>
#include <chrono>
#include <thread>

#include <QtSql>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QCoreApplication>

#include "LicensePlateGenerator.h"

using namespace std;

mutex mtx;

string models[] = {"Tesla","Mercedes","Hyundai","Mitsubishi","BMW"};
string owners[] = {"Flemming","Gunder","Frederik","Jannik","Peter"};

LicensePlateGenerator plateGen;

std::uniform_int_distribution<int> manufacturer(0, 4);
std::uniform_int_distribution<int> year(1900, 2021);
std::random_device rd;
std::default_random_engine randGen(rd());

void generateData(char* numberPlate) {
    for (int i = 0; i < 20000; i++) {
        plateGen.generate(numberPlate);
        cout << numberPlate << " " << models[manufacturer(randGen)] << " "
             << year(randGen) << " " << owners[manufacturer(randGen)] << endl;

        //INSERT INTO bil (registreringsnr, model, aargang, ejer)
        //VALUES (numberPlate, modeler[distribution(generator)], aargang(generator), ejerer[distribution(generator)]);
    }
}

void registerCar(char* numberPlate) {
    mtx.lock();

    mtx.unlock();
}

void removeCar() {
    mtx.lock();

    mtx.unlock();
}

void reRegisterCar(char* numberPlate) {
    mtx.lock();

    mtx.unlock();
}


int main()
{
    char* numberPlate = new char[7];

    generateData(numberPlate);

    thread adder{registerCar, numberPlate};
    thread deleter{removeCar};
    thread reRegister{reRegisterCar, numberPlate};

    adder.join();
    deleter.join();
    reRegister.join();

    //    for (int i = 0; i < 200100; i++) {
    //        NumberPlateGenerator.generate(numberPlate);
    //        cout << numberPlate << endl;
    //    }

    return 0;
}
