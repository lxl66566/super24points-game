
#ifndef OPERATION_H
#define OPERATION_H
#include <functional>
using i32 = int;
enum operations{
    add,minus,times,divide,and_,or_,not_,power,divide_exactly
};

class operation
{
private:
    operations op_this;
    std::function<i32(i32,i32)> op;

public:
    operation();
};

#endif // OPERATION_H
