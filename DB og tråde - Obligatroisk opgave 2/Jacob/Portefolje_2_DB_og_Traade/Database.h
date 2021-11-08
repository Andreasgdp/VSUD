#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <string>

class Database
{
public:
    Database();
    void createDatabase();
    void updateTotalPlateNumberReached(int totalPlateNumberReached);
    int getTotalPlateNumberReached();
    bool isEmpty();
    void insertCar(char* numberPlate, std::string model, int aargang, std::string ejer);

private:
    QSqlDatabase _db;
};

#endif // DATABASE_H
