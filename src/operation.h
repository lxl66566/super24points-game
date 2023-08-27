#ifndef OPERATION_H
#define OPERATION_H
#include <QPushButton>
#include <functional>
#include <variant>

using i32 = int;
using f64 = double;
enum operations {
  add,
  minus,
  times,
  divide,
  and_,
  or_,
  xor_,
  power,
  divide_exactly
};

class operation {
private:
  operations op;
  QPushButton *button;
  bool activated = false;

public:
  operation(operations, std::function<f64(f64, f64)>, QPushButton *);
  std::function<f64(f64, f64)> func; // without a type check
  std::variant<f64, QString>
      dynamic_func(f64, f64); // type check, return QString err if invalid
  std::variant<f64, QString> dynamic_func(std::variant<f64, QString>, f64);
  // type check, return QString err if invalid
  static QString op_string(operations);
  QString get_op_string();
  i32 get_op_rank();
  operations get_operation() const;
  QPushButton *get_button();
  bool is_activated();
  bool accept_f64_calculation();
  bool accept_exchange();
  void set_activated(bool);
  void deactivated();
  bool operator==(operation const &) const;
};

#endif // OPERATION_H
