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

using namespace std;

mutex mtx;

string modeler[] = {"Audi","BMW","VW","Mercedes","Volvo"};
string ejerer[] = {"Jens","Peter","Knud","Hans","Egon"};

NumberPlateGenerator NumberPlateGenerator;

std::uniform_int_distribution<int> distribution(0, 4);
std::uniform_int_distribution<int> aargang(1900, 2021);
std::random_device rd;
std::default_random_engine generator(rd());

void generateData(char* numberPlate) {
    mtx.lock();
    for (int i = 0; i < 20000; i++) {
        NumberPlateGenerator.generate(numberPlate);
        cout << numberPlate << " " << modeler[distribution(generator)] << " "
             << aargang(generator) << " " << ejerer[distribution(generator)] << endl;

        //INSERT INTO bil (registreringsnr, model, aargang, ejer)
        //VALUES (numberPlate, modeler[distribution(generator)], aargang(generator), ejerer[distribution(generator)]);
    }
    mtx.unlock();
}

void addCar(char* numberPlate) {

}

void removeCar() {

}

void reRegisterCar(char* numberPlate) {

}


int main()
{
    char* numberPlate = new char[7];

    generateData(numberPlate);

    thread adder{addCar, numberPlate};
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
