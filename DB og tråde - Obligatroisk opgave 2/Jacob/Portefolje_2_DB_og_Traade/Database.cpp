#include "Database.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QSqlDriver>
#include <iostream>
#include <cstring>

using namespace std;

template <
    class result_t   = std::chrono::milliseconds,
    class clock_t    = std::chrono::steady_clock,
    class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

Database::Database() {
    _db = QSqlDatabase::addDatabase("QMYSQL");
    _db.setHostName("localhost");
    _db.setDatabaseName("motorRegistre");
    _db.setUserName("user1");
    _db.setPassword("password1");
}

void Database::createDatabase() {
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


void Database::dropDatabase() {
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

void Database::createIndex() {
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

int Database::getLenghtOfTable() {
    int lenght = 0;
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);
        query.prepare("SELECT COUNT(*) FROM bil;");
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

bool Database::doesNumberPlateAlreadyExist(char *numberPlate) {
    bool success = false;
    bool exist = 0;
    if (_db.open()) {
        QSqlQuery query(_db);
        query.prepare("SELECT COUNT(1) FROM bil WHERE registreringsnr = :numberPlate;");
        query.bindValue(":numberPlate", numberPlate);
        success = query.exec();

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }

        if (query.next()) {
            exist = query.value(0).toInt();
        }
    }
    return exist;
}

void Database::insertCar(char* numberPlate, std::string model, int aargang, std::string ejer) {
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
        query.bindValue(":registreringsnr", numberPlate);
        query.bindValue(":model", QString::fromStdString(model));
        query.bindValue(":aargang", aargang);
        query.bindValue(":ejer", QString::fromStdString(ejer));
        success = query.exec();

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}

void Database::removeCar(int id) {
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

void Database::reRegistreCar(std::string ejer, std::string model, std::string kober) {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);

        query.prepare("UPDATE bil SET ejer = :kober WHERE ejer = :ejer AND model = :model;");
        query.bindValue(":kober", QString::fromStdString(kober));
        query.bindValue(":ejer", QString::fromStdString(ejer));
        query.bindValue(":model", QString::fromStdString(model));
        success = query.exec();

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}

void Database::performanceTest(std::string ejer, std::string model) {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);

        query.prepare("SELECT COUNT(*) FROM bil WHERE ejer = :ejer AND model = :model;");
        query.bindValue(":ejer", QString::fromStdString(ejer));
        query.bindValue(":model", QString::fromStdString(model));

        auto start = std::chrono::steady_clock::now();
        success = query.exec();
        double svartid = static_cast<double>(since<std::chrono::microseconds>(start).count()) / 1000;

        std::cout << "Elapsed(ms)=" << svartid << endl << endl;
        svartider.push_back(svartid);

        if (!success) {
            qDebug() << query.lastError();
            _db.rollback();
        }
    }
}

const std::vector<double> &Database::getSvartider() const {
    return svartider;
}
