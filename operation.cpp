
#include "operation.h"


operation::operation(operations op, std::function<i32 (i32, i32)> func, QPushButton *button)
    : op(op),func(func),button(button)
{

}

QString operation::op_string(operations op)
{
    switch(op)
    {
    case add:
        return QString("+");
    case minus:
        return QString("-");
    case times:
        return QString("*");
    case divide:
        return QString("/");
    case and_:
        return QString("&");
    case or_:
        return QString("|");
    case xor_:
        return QString("^");
    case divide_exactly:
        return QString("//");
    case power:
        return QString("^");
    default:
        return QString("");
    }
}

QString operation::get_op_string()
{
    return op_string(this->op);
}

operations operation::get_operation() const
{
    return op;
}

QPushButton* operation::get_button()
{
    return button;
}

bool operation::is_activated()
{
    return activated;
}

void operation::set_activated(bool temp)
{
    activated = temp;
    if (temp)
        button->show();
    else button->hide();
}

void operation::deactivated()   // no used
{
    activated = false;
    button->hide();
}
