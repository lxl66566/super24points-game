#include "pop_up_window.h"

pop_up_window::pop_up_window(QWidget *parent) : QMessageBox(parent) {}
pop_up_window::pop_up_window(QWidget *parent, QString window_title,
                             QString text, QPixmap pixmap)
    : QMessageBox(parent) {
  setWindowFlags(Qt::Dialog);
  setWindowTitle(window_title);
  setText(text);
  setIconPixmap(pixmap);
  setWindowIcon(QIcon(":/static/logo.ico"));
  ok = addButton("close", QMessageBox::RejectRole);
  next = addButton("new game", QMessageBox::YesRole);
}

void pop_up_window::pop_up() {
  exec();
  if (clickedButton() == ok)
    emit Ok();
  else
    emit Next();
}

void pop_up_window::set_button_text(QString s1, QString s2) {
  ok->setText(s1);
  next->setText(s2);
}

void pop_up_window::set_text(QString s) { setText(s); }
