#ifndef API_H
#define API_H
#include <QSqlDatabase>
#include <string>
#include "LicensePlateGenerator.h"

class API
{
public:
    API(std::string connectionName = "default");
    void createTable();
    void createIndex();
    void dropTable();

    int getLenghtOfTable();
    bool licensePlateAlreadyExist(char* numberPlate);

    void insertCar(char* licensePlate, std::string model, int year, std::string owner);
    void removeCar(int id);
    void reRegistreCar(std::string owner, std::string model, std::string buyer);
    void performanceTest(std::string owner, std::string model);

    const std::vector<double> &getResponseTimes() const;

private:
    LicensePlateGenerator _lpg;
    QSqlDatabase _db;
    std::vector<double> _responseTimes;
};

#endif // API_H
