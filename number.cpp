
#include "number.h"

number::number(i32 i, QPushButton *b) : num(i) , button(b)
{
    clear();
}

i32 number::get_num()
{
    return num;
}

QString number::get_string_num()
{
    return QString::number(num);
}

void number::clear()    // recover
{
    button->setCheckable(true);
    button->setText(this->get_string_num());
}

void number::use()
{
    button->setCheckable(false);
    button->setText(QString());
}

void number::transfer(QPushButton *b)   // useless
{
    use();
    button = b;
    clear();
}
