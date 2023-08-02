
#include "operation.h"
#include "number.h"

operation::operation(operations op, std::function<f64(f64, f64)> func,
                     QPushButton *button)
    : op(op), func(func), button(button) {}

std::variant<f64, QString> operation::dynamic_func(f64 x, f64 y) {
  try {
    if ((!number::is_i32(x) || !number::is_i32(y)) && !accept_f64_calculation())
      throw QString("Cannot do this operation to a float number!");
    if (y == 0 && (op == divide || op == divide_exactly))
      throw QString("Division by zero condition!");
    return func(x, y);
  } catch (QString err) {
    return err;
  }
}

std::variant<f64, QString> operation::dynamic_func(std::variant<f64, QString> x,
                                                   f64 y) {
  try {
    return dynamic_func(std::get<f64>(x), y);
  } catch (std::bad_variant_access &) {
    return std::get<QString>(x);
  }
}

QString operation::op_string(operations op) {
  switch (op) {
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

QString operation::get_op_string() { return op_string(this->op); }

i32 operation::get_op_rank() {
  switch (op) {
  case add:
  case minus:
    return 1;
  case times:
  case divide:
    return 2;
  case divide_exactly:
    return 3;
  case and_:
    return 4;
  case or_:
    return 5;
  case xor_:
    return 6;
  case power:
    return 7;
  default:
    return 8;
  }
}

operations operation::get_operation() const { return op; }

QPushButton *operation::get_button() { return button; }

bool operation::is_activated() { return activated; }

bool operation::accept_f64_calculation() {
  switch (op) {
  case add:
  case minus:
  case times:
  case divide:
  case power:
    return true;
  default:
    return false;
  }
}

bool operation::accept_exchange() {
  switch (op) {
  case add:
  case times:
  case xor_:
  case and_:
  case or_:
    return true;
  default:
    return false;
  }
}

void operation::set_activated(bool temp) {
  activated = temp;
  if (temp)
    button->show();
  else
    button->hide();
}

void operation::deactivated() // no used
{
  activated = false;
  button->hide();
}

bool operation::operator==(const operation &temp) const {
  return op == temp.op;
}
