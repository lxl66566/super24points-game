
#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <vector>
#include <QString>
#include <ranges>
#include "number.h"
#include "operation.h"
using i32 = int;
const i32 TARGET_NUMBER = 24;

class calculator
{
private:
    std::vector<i32> num;
    std::vector<operation> all_operation;
    QString answer;
public:
    calculator();
    calculator(std::vector<i32>);
    calculator(std::vector<number>,std::vector<operation>);
    QString calculate();
    QString get_ans();
};

#endif // CALCULATOR_H
