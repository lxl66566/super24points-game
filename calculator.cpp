
#include "calculator.h"

calculator::calculator()
{

}

QString calculator::calculate(std::vector<int> num)
{
    QString ans;
    // do sth
    return ans;
}

QString calculator::calculate(std::vector<number> numbers)
{
    std::vector<i32> temp(4);
    for (auto i : numbers) temp.push_back(i.get_num());
    return calculate(temp);
}
