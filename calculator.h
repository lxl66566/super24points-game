
#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <vector>
#include <QString>
#include "number.h"
using i32 = int;
class calculator
{
public:
    calculator();
    static QString calculate(std::vector<int>);
    static QString calculate(std::vector<number>);
};

#endif // CALCULATOR_H
