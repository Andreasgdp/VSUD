#ifndef NUMBERPLATEGENERATOR_H
#define NUMBERPLATEGENERATOR_H

class NumberPlateGenerator
{
public:
    NumberPlateGenerator();
    ~NumberPlateGenerator();
    void generate(char* numberPlate);

private:
    static inline int _plateNumberReached;
    static inline int _totalPlateNumberReached;
};

#endif // NUMBERPLATEGENERATOR_H
