
#include "number.h"

number::number(i32 i, QPushButton *b) : num(i) , bottom(b)
{

}

i32 number::get_num()
{
    return num;
}

QString number::get_string_num()
{
    return QString::number(num);
}

void number::clear()
{
    bottom->setCheckable(true);
}

void number::use()
{
    bottom->setCheckable(false);
}
