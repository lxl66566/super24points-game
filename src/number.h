#ifndef NUMBER_H
#define NUMBER_H

#include <QPushButton>
#include <functional>

using i32 = int;
using f64 = double;

class number {
private:
  f64 num;

protected:
  QPushButton *button;

public:
  number();
  number(i32 i, QPushButton *b);
  number(f64 i, QPushButton *b);
  f64 get_num();
  QString get_string_num();
  QPushButton *get_button();
  void clear();
  void use();
  void transfer(QPushButton *);
  bool operator<(const number &) const;
  bool is_i32() const;
  static bool is_i32(number i);
  static bool is_i32(double i);
};

// class number_double : public number
//{
// private:
//     f64 num;
// public:
//     number_double(f64 num, QPushButton* button);
//     f64 get_num();
// };

#endif // NUMBER_H
