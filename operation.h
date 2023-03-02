
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
    QPushButton *button;
    bool activated = false;
public:
    std::function<i32(i32,i32)> func;
    operation(operations,std::function<i32(i32,i32)>,QPushButton *);
    static QString op_string(operations);
    QString get_op_string();
    operations get_operation() const;
    QPushButton* get_button();
    bool is_activated();
    void set_activated(bool);
    void deactivated();
};

#endif // OPERATION_H
