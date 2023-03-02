
#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <vector>
#include <QString>
using i32 = int;
class calculator
{
public:
    calculator();
    static QString calculate(std::vector<int> num);
};

#endif // CALCULATOR_H
