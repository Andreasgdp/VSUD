#include "NumberPlateGenerator.h"
#include "Database.h"
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

Database dab;

NumberPlateGenerator::NumberPlateGenerator() {
    //dab.updateTotalPlateNumberReached(0);
    _totalPlateNumberReached = dab.getTotalPlateNumberReached();
    cout << "Works here" << endl;
    _plateNumberReached = _totalPlateNumberReached % 100000;
}

NumberPlateGenerator::~NumberPlateGenerator() {
    dab.updateTotalPlateNumberReached(_totalPlateNumberReached);
}

void NumberPlateGenerator::generate(char* numberPlate) {
    char characters[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

    int firstLetter = _totalPlateNumberReached / (100000 * strlen(characters));
    int secondLetter = _totalPlateNumberReached / 100000;

    numberPlate[0] = characters[firstLetter];
    numberPlate[1] = characters[secondLetter];

    int plateNumberLenght = to_string(_plateNumberReached).length();
    for (int i = 2; i < 7 - plateNumberLenght; i++) {
        numberPlate[i] = '0';
    }

    string stringPlateNumber = to_string(_plateNumberReached);
    for (int i = 0; i < plateNumberLenght; i++) {
        numberPlate[7 - plateNumberLenght + i] = stringPlateNumber[i];
    }

    _totalPlateNumberReached += 1;
    _plateNumberReached = _totalPlateNumberReached % 100000;
}
