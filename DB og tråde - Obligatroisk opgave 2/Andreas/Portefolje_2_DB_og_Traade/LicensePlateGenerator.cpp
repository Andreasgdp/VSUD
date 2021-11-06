#include "LicensePlateGenerator.h"
#include <string>
#include <cstring>

using namespace std;

LicensePlateGenerator::LicensePlateGenerator()
{
    _maxPlateNumReached = 0;
    _totalPlateAmtReached = 0;
}

void LicensePlateGenerator::generate(char *licensePlate)
{
    int maxNumCombinations = 100000;

    char chars[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

    // Generate the 2 first letters
    int firstLetter = _totalPlateAmtReached / (maxNumCombinations * strlen(chars));
    int secondLetter = _totalPlateAmtReached / maxNumCombinations;
    licensePlate[0] = chars[firstLetter];
    licensePlate[1] = chars[secondLetter];

    // Fill in zeros in the fields that will not contain a number
    // e.g. the number 14 will be 00 014 i.e. three zeros
    int plateNumLen = to_string(_maxPlateNumReached).length();
    for (int i = 2; i < 7 - plateNumLen; i++) {
        licensePlate[i] = '0';
    }

    // Fill the remaining numbers
    string stringPlateNumber = to_string(_maxPlateNumReached);
    for (int i = 0; i < plateNumLen; i++) {
        licensePlate[7 - plateNumLen + i] = stringPlateNumber[i];
    }

    _totalPlateAmtReached += 1;
    _maxPlateNumReached = _totalPlateAmtReached % maxNumCombinations;
}
