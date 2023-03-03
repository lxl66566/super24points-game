#include <algorithm>
#include <QDebug>
#include "calculator.h"

calculator::calculator()
{}

calculator::calculator(std::vector<i32> i) : num(i)
{
    calculate();
}

calculator::calculator(std::vector<number> i, std::vector<operation> ops)
{
    std::ranges::for_each(i,[this](number &i){num.push_back(i.get_num());});
    std::ranges::for_each(ops,[this](operation &i){if(i.is_activated()) all_operation.push_back(i);});
    // i don't know why the code above could run well but the code behind couldn't.
//    all_operation.erase(std::remove_if(all_operation.begin(),all_operation.end(),
//        [](operation &i){return !i.is_activated();}));
    calculate();
}

QString calculator::calculate()
{
    std::ranges::sort(num);
    std::vector<QString> ans;
    for (auto &op1 : all_operation)
    for (auto &op2 : all_operation)
    for (auto &op3 : all_operation)
    {
        do
        {
            try {
                auto temp = op3.dynamic_func(op2.dynamic_func(op1.func(num.at(0),num.at(1)),num.at(2)),num.at(3));
                try{
                    if (std::get<f64>(temp)  == TARGET_NUMBER)
                        ans.push_back(QString("((%1 %2 %3) %4 %5) %6 %7").arg(num[0]).arg(op1.get_op_string()).arg(num[1])
                                          .arg(op2.get_op_string()).arg(num[2]).arg(op3.get_op_string()).arg(num[3]));
                }catch(std::bad_variant_access&){
                    throw std::get<QString>(temp);
                }
            } catch (QString err) {
//                qDebug() << QString("((%1 %2 %3) %4 %5) %6 %7").arg(num[0]).arg(op1.get_op_string()).arg(num[1])
//                                .arg(op2.get_op_string()).arg(num[2]).arg(op3.get_op_string()).arg(num[3]);
            }
        }while(std::next_permutation(num.begin(),num.end()));
        std::ranges::sort(num);
        std::swap(num.at(0),num.at(2));
        std::swap(num.at(1),num.at(3));
        do
        {
            try{
                auto temp = op2.dynamic_func(op1.func(num.at(0),num.at(1)),op3.func(num.at(2),num.at(3)));
                try{
                    if (std::get<f64>(temp)  == TARGET_NUMBER)
                        ans.push_back(QString("(%1 %2 %3) %4 (%5 %6 %7)").arg(num[0]).arg(op1.get_op_string()).arg(num[1])
                                            .arg(op2.get_op_string()).arg(num[2]).arg(op3.get_op_string()).arg(num[3]));
                }catch(std::bad_variant_access&){
                    throw std::get<QString>(temp);
                }
            }catch(QString err)
            {
//                qDebug() << QString("(%1 %2 %3) %4 (%5 %6 %7)").arg(num[0]).arg(op1.get_op_string()).arg(num[1])
//                                .arg(op2.get_op_string()).arg(num[2]).arg(op3.get_op_string()).arg(num[3]);
            }
        }while(std::next_permutation(num.begin(),num.end()));
    }
    size_t temp = 0;
    QString output;
    for (auto &i : ans)
    {
        output += i;
        ++temp;
        if (temp % 2 == 1)
            output += "\t";
        else output += "\n";
    }
    if (answer.isEmpty()) answer = output;
    return output;
}

QString calculator::get_ans()
{
    return answer;
}
