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
#include "API.h"

using namespace std;

mutex mtx;
LicensePlateGenerator plateGen;
API api;

string models[] = {"Tesla", "Mercedes", "Hyundai", "Mitsubishi", "BMW"};
string owners[] = {"Flemming", "Gunder", "Frederik", "Jannik", "Peter"};


std::uniform_int_distribution<int> randDist(0, 4);
std::uniform_int_distribution<int> years(1900, 2021);
std::random_device rd;
std::default_random_engine randGen(rd());

const int numCarsToGenerate = 20000;

bool carryon()
{
    bool returnVal = false;
    mtx.lock();
    returnVal = (api.getLenghtOfTable() < numCarsToGenerate) ? true : false;
    mtx.unlock();
    return returnVal;
}

// Ved registrering af bil
void insertCar(char *numberPlate, API api, int sleepTime)
{
    while (carryon())
    {
        mtx.lock();
        plateGen.generateNext(numberPlate);
        string model = models[randDist(randGen)];
        int year_var = years(randGen);
        string ejer = owners[randDist(randGen)];
        cout << "insert: "
             << "(" << numberPlate << ") " << endl;
        mtx.unlock();
        api.insertCar(numberPlate, model, year_var, ejer);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

// Ved skrotning af bil
void removeCar(API api)
{
    while (carryon())
    {
        mtx.lock();
        std::uniform_int_distribution<int> selectRandomNumberplateId(0, api.getLenghtOfTable());
        int randomNumberPlateId = selectRandomNumberplateId(randGen);
        cout << "remove: "
             << "(Id: " << randomNumberPlateId << ") " << endl;;
        api.removeCar(randomNumberPlateId);
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

// Change of car owners (Re-register all of one car type from one person to another)
void reRegisterCar(API api)
{
    while (carryon())
    {
        mtx.lock();
        string owner = "";
        string buyer = "";
        string model = models[randDist(randGen)];
        // ensure the owner is not also the buyer
        do
        {
            owner = owners[randDist(randGen)];
            buyer = owners[randDist(randGen)];
        } while (owner == buyer);
        cout << "re-registre: "
             << "(" << owner << ", " << model << ", " << buyer << ") " << endl;;
        api.reRegistreCar(owner, model, buyer);
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main()
{
    api.createTable();
    char *numberPlate = new char[7];

    cout << "Clear table 'bil'? (y/n): ";
    string answer;
    cin >> answer;
    if (answer == "y")
    {
        cout << "clearing... " << endl << endl;
        api.dropTable();
        api.createTable();
        cout << "Generate 20000 cars or run multiple threads (inserter, remover, re-register)? (y: for generete / n: for milti action): ";
        string answer;
        cin >> answer;
        if (answer == "y")
        {
            thread inserter{insertCar, numberPlate, API("api1"), 0};
            thread inserter2{insertCar, numberPlate, API("api2"), 0};
            thread inserter3{insertCar, numberPlate, API("api3"), 0};

            inserter.join();
            inserter2.join();
            inserter3.join();

            cout << "Create index? (y/n): ";
            string answer;
            cin >> answer;
            if (answer == "y")
            {
                cout << "creating index... " << endl
                     << endl;
                api.createIndex();
            }

            else if (answer == "n")
            {
            }
            else
            {
                throw invalid_argument("You can only answer y or n!");
            }
        }
        else if (answer == "n")
        {
        }
        else
        {
            throw invalid_argument("You can only answer y or n!");
        }
    }
    else if (answer == "n")
    {
    }
    else
    {
        throw invalid_argument("You can only answer y or n!");
    }

    thread inserter{insertCar, numberPlate, API("api4"), 100};
    thread remover{removeCar, API("api5")};
    thread reRegister{reRegisterCar, API("api6")};

    inserter.join();
    remover.join();
    reRegister.join();

    cout << "Performance check? (y/n): ";
    cin >> answer;
    if (answer == "y")
    {
        for (int i = 0; i < 100; i++) {
            api.performanceTest(owners[randDist(randGen)], models[randDist(randGen)]);
        }
        vector<double> responseTime = api.getResponseTimes();
        double totalResponseTime = 0;
        for (int i = 0; i < responseTime.size(); i++)
        {
            totalResponseTime += responseTime[i];
        }
        cout << "avg. response time(ms): " << totalResponseTime / responseTime.size() << endl
             << endl;
    }
    else if (answer == "n")
    {
    }
    else
    {
        throw invalid_argument("You can only answer y or n!");
    }
    return 0;
}
