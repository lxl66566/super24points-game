#ifndef NUMBER_H
#define NUMBER_H

#include <functional>
#include <QPushButton>
using i32 = int;
class number
{
private:
    i32 num;
    QPushButton *bottom;

public:
    number(i32 i,QPushButton *b);
    i32 get_num();
    QString get_string_num();
    void clear();
    void use();
};

#endif // NUMBER_H