
#ifndef OPERATION_H
#define OPERATION_H
#include <functional>
#include <QPushButton>

using i32 = int;
enum operations{
    add,minus,times,divide,and_,or_,xor_,power,divide_exactly
};

class operation
{
private:
    operations op;
    std::function<i32(i32,i32)> func;
    QPushButton *button;
    bool activated = false;
public:
    operation(operations,std::function<i32(i32,i32)>,QPushButton *);
    static QString op_string(operations);
    QString get_op_string();
    operations get_operation() const;
    bool is_activated();
    void set_activated(bool);
};

#endif // OPERATION_H
