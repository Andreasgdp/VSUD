#ifndef LICENSEPLATEGENERATOR_H
#define LICENSEPLATEGENERATOR_H


class LicensePlateGenerator
{
public:
    LicensePlateGenerator();
    void generateNext(char* licensePlate);

private:
    // Static in order to prevent possible multible instances
    // generating new plates identical plates
    static inline int _maxPlateNumReached;
    static inline int _totalPlateAmtReached;
};

#endif // LICENSEPLATEGENERATOR_H
