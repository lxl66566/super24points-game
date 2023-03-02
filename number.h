#ifndef NUMBER_H
#define NUMBER_H

#include <functional>
#include <QPushButton>
using i32 = int;
class number
{
private:
    i32 num;
    QPushButton *button;

public:
    number(i32 i,QPushButton *b);
    i32 get_num();
    QString get_string_num();
    QPushButton * get_button();
    void clear();
    void use();
    void transfer(QPushButton*);
};

#endif // NUMBER_H
