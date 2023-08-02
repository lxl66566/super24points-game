#include "number.h"

number::number() {}

number::number(i32 i, QPushButton *b) : num(static_cast<f64>(i)), button(b) {
  clear();
}

number::number(f64 i, QPushButton *b) : num(i), button(b) { clear(); }

f64 number::get_num() { return num; }

QString number::get_string_num() { return QString::number(num); }

QPushButton *number::get_button() { return button; }

void number::clear() // recover
{
  button->setDisabled(false);
  button->setText(this->get_string_num());
  //    qDebug() << "number: " << num << "has been cleared;";
}

void number::use() {
  button->setDisabled(true);
  button->setText(QString());
  //    qDebug() << "number: " << num << "has been used;";
}

void number::transfer(QPushButton *b) // useless
{
  use();
  button = b;
  clear();
}

bool number::operator<(const number &x) const { return this->num < x.num; }

bool number::is_i32() const {
  return num == static_cast<f64>(static_cast<i32>(num));
}

bool number::is_i32(number i) { return number::is_i32(i.get_num()); }

bool number::is_i32(double i) {
  return i == static_cast<f64>(static_cast<i32>(i));
}

// number_double::number_double(f64 num, QPushButton * b) : num(num)
//{
//     button = b;
// }
