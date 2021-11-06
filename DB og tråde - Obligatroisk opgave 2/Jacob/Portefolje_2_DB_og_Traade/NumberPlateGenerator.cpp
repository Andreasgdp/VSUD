#include "NumberPlateGenerator.h"
#include <string>
#include <cstring>

using namespace std;

NumberPlateGenerator::NumberPlateGenerator() {
    _plateNumberReached = 0;
    _totalPlateNumberReached = 0;
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
