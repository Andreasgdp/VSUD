#include "API.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QSqlDriver>
#include <iostream>
#include <cstring>

using namespace std;

// https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
template <
    class result_t   = std::chrono::milliseconds,
    class clock_t    = std::chrono::steady_clock,
    class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

API::API(string connectionName) {
    _db = QSqlDatabase::addDatabase("QMYSQL", QString::fromStdString(connectionName));
    _db.setHostName("localhost");
    _db.setDatabaseName("biler");
    _db.setUserName("user1");
    _db.setPassword("password1");
}

void API::createTable() {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);
        success = query.exec("CREATE TABLE IF NOT EXISTS bil ( "
                             "id int NOT NULL AUTO_INCREMENT, "
                             "registreringsnr char(7) NOT NULL, "
                             "model char(20) NOT NULL, "
                             "aargang int NOT NULL,"
                             "ejer char(20) NOT NULL,"
                             "PRIMARY KEY (id),"
                             "UNIQUE (registreringsnr)"
                             "); ");

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}

void API::createIndex() {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);
        success = query.exec("CREATE INDEX ejer_model_idx ON bil (ejer, model);");

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}

void API::dropTable() {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);
        success = query.exec("DROP TABLE IF EXISTS bil;");

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}



int API::getLenghtOfTable() {
    int lenght = 0;
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);
        query.prepare("SELECT COUNT(id) FROM bil;");
        success = query.exec();

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }

        if (query.next()) {
            lenght = query.value(0).toInt();
        }
    }
    return lenght;
}

bool API::licensePlateAlreadyExist(char *licensePlate) {
    bool success = false;
    bool exist = 0;
    if (_db.open()) {
        QSqlQuery query(_db);
        query.prepare("SELECT COUNT(1) FROM bil WHERE registreringsnr = :numberPlate;");
        query.bindValue(":numberPlate", licensePlate);
        success = query.exec();

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }

        if (query.next()) {
            exist = query.value(0).toBool();
        }
    }
    return exist;
}

void API::insertCar(char* licensePlate, std::string model, int year, std::string owner) {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);
        query.prepare("INSERT INTO bil (registreringsnr, "
                                        "model, "
                                        "aargang, "
                                        "ejer)"
                                   "VALUES (:registreringsnr, "
                                        ":model, "
                                        ":aargang, "
                                        ":ejer);");
        query.bindValue(":registreringsnr", licensePlate);
        query.bindValue(":model", QString::fromStdString(model));
        query.bindValue(":aargang", year);
        query.bindValue(":ejer", QString::fromStdString(owner));
        success = query.exec();

        if (!success) {
//            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}

void API::removeCar(int id) {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);
        query.prepare("DELETE FROM bil WHERE id = :id;");
        query.bindValue(":id", id);
        success = query.exec();

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}

void API::reRegistreCar(std::string owner, std::string model, std::string buyer) {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);

        query.prepare("UPDATE bil SET ejer = :kober WHERE ejer = :ejer AND model = :model;");
        query.bindValue(":kober", QString::fromStdString(buyer));
        query.bindValue(":ejer", QString::fromStdString(owner));
        query.bindValue(":model", QString::fromStdString(model));
        success = query.exec();

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}

void API::performanceTest(std::string owner, std::string model) {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);

        query.prepare("SELECT COUNT(*) FROM bil WHERE ejer = :ejer AND model = :model;");
        query.bindValue(":ejer", QString::fromStdString(owner));
        query.bindValue(":model", QString::fromStdString(model));

        auto start = std::chrono::steady_clock::now();
        success = query.exec();
        double responseTime = static_cast<double>(since<std::chrono::microseconds>(start).count()) / 1000;

        std::cout << "Elapsed time(ms)=" << responseTime << endl << endl;
        _responseTimes.push_back(responseTime);

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}

const std::vector<double> &API::getResponseTimes() const {
    return _responseTimes;
}
