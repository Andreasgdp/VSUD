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
string ejerer[] = {"Jens","Erik","Knud","Hans","Egon"};

NumberPlateGenerator NumberPlateGenerator;
Database db;

std::uniform_int_distribution<int> distribution(0, 4);
std::uniform_int_distribution<int> aargang(1900, 2021);
std::random_device rd;
std::default_random_engine generator(rd());

void generateData(char* numberPlate) {
    cout << "generating data... " << endl << endl;
    for (int i = 0; i < 20000; i++) {
        do {
            NumberPlateGenerator.generate(numberPlate);
        } while (db.doesNumberPlateAlreadyExist(numberPlate));
        string model = modeller[distribution(generator)];
        int aargang_var = aargang(generator);
        string ejer = ejerer[distribution(generator)];
        db.insertCar(numberPlate, model, aargang_var, ejer);
    }

    cout << "Create index? (y/n): ";
    string answer;
    cin >> answer;
    if (answer == "y") {
        cout << "creating index... " << endl << endl;
        db.createIndex();
    } else if (answer == "n") {

    } else {
        throw invalid_argument("You can only answer y or n!");
    }
}

bool carryon() {
    mtx.lock();
    if (db.getLenghtOfTable() < 20000) {
        mtx.unlock();
        return true;
    } else {
        mtx.unlock();
        return false;
    }
}

// Ved registrering af bil
void insertCar(char* numberPlate) {
    while (carryon()) {
        mtx.lock();
        do {
            NumberPlateGenerator.generate(numberPlate);
        } while (db.doesNumberPlateAlreadyExist(numberPlate));
        string model = modeller[distribution(generator)];
        int aargang_var = aargang(generator);
        string ejer = ejerer[distribution(generator)];
        cout << "insert... " << "(" << numberPlate << ") ";
        db.insertCar(numberPlate, model, aargang_var, ejer);
        cout << "done" << endl;
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Ved skrotning af bil
void removeCar() {
    while (carryon()) {
        mtx.lock();
        std::uniform_int_distribution<int> selectRandomNumberplateId(0, db.getLenghtOfTable());
        int randomNumberPlateId = selectRandomNumberplateId(generator);
        cout << "remove... " << "(Id: " << randomNumberPlateId << ") ";
        db.removeCar(randomNumberPlateId);
        cout << "done" << endl;
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

// Ved ejerskifte af bil
void reRegisterCar() {
    while (carryon()) {
        mtx.lock();
        string ejer = "";
        string kober = "";
        string model = modeller[distribution(generator)];
        do {
            ejer = ejerer[distribution(generator)];
            kober = ejerer[distribution(generator)];
        } while (ejer == kober);
        cout << "re-registre... " << "(" << ejer << ", " << model << ", " << kober << ") ";
        db.reRegistreCar(ejer, model, kober);
        cout << "done" << endl;
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}


int main()
{
    db.createDatabase();
    char* numberPlate = new char[7];

    cout << "Clear table 'bil'? (y/n): ";
    string answer;
    cin >> answer;
    if (answer == "y") {
        cout << "clearing... " << endl << endl;
        db.dropDatabase();
        db.createDatabase();
        cout << "Generate data (fast method)? (y/n): ";
        string answer;
        cin >> answer;
        if (answer == "y") {
            generateData(numberPlate);
        } else if (answer == "n") {

        } else {
            throw invalid_argument("You can only answer y or n!");
        }
    } else if (answer == "n") {

    } else {
        throw invalid_argument("You can only answer y or n!");
    }

    thread inserter{insertCar, numberPlate};
    thread remover{removeCar};
    thread reRegister{reRegisterCar};

    inserter.join();
    remover.join();
    reRegister.join();

    while (!carryon()) {
        cout << "Do performance check? (y/n): ";
        string answer;
        cin >> answer;
        if (answer == "y") {
            db.performanceTest(ejerer[distribution(generator)], modeller[distribution(generator)]);
        } else if (answer == "n") {
            vector<double> svartider = db.getSvartider();
            if (svartider.size() > 0) {
                double totalSvartid = 0;
                for (int i = 0; i < svartider.size(); i++) {
                    totalSvartid += svartider[i];
                }
                cout << "gns svartid(ms): " << totalSvartid / svartider.size() << endl << endl;
            }
            break;
        } else {
            throw invalid_argument("You can only answer y or n!");
        }
    }

    return 0;
}
