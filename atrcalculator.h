#ifndef ATRCALCULATOR_H
#define ATRCALCULATOR_H

#include <QString>

class ATRCalculator
{
public:
    static double calculateATR(const QString& symbol, int period = 14);
};

#endif // ATRCALCULATOR_H
