#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <string>

class Database
{
public:
    Database();
    void createDatabase();
    void dropDatabase();
    void createIndex();

    int getLenghtOfTable();
    bool doesNumberPlateAlreadyExist(char* numberPlate);

    void insertCar(char* numberPlate, std::string model, int aargang, std::string ejer);
    void removeCar(int id);
    void reRegistreCar(std::string ejer, std::string model, std::string kober);
    void performanceTest(std::string ejer, std::string model);

    const std::vector<double> &getSvartider() const;

private:
    QSqlDatabase _db;
    std::vector<double> svartider;
};

#endif // DATABASE_H
