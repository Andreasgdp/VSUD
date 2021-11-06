#include "Database.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QSqlDriver>
#include <iostream>
#include <cstring>

using namespace std;

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
                             "bil_id int NOT NULL AUTO_INCREMENT, "
                             "registreringsnr char(7) NOT NULL, "
                             "model char(20) NOT NULL, "
                             "aargang int NOT NULL,"
                             "ejer char(20) NOT NULL,"
                             "PRIMARY KEY (bil_id),"
                             "UNIQUE (registreringsnr)"
                             "); ");
    }
    if (!success)
        throw std::invalid_argument("Something went wrong in creation of DB!");
}

bool Database::isEmpty() {
    bool success = false;
    if (_db.open()) {
        QSqlQuery query(_db);
        success = query.exec("SELECT CASE WHEN EXISTS(SELECT 1 FROM bil) THEN 0 ELSE 1 END AS IsEmpty;");

        if (!success)
            throw std::invalid_argument("Something went wrong in isEmpty!");

        if (query.next()) {
            return query.value(0).toInt();
        }
    }
    return 1;
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
    }
    if (!success)
        throw std::invalid_argument("Something went wrong in insert!");

}
