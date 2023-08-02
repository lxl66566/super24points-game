
#ifndef POP_UP_WINDOW_H
#define POP_UP_WINDOW_H
#include <QAbstractButton>
#include <QMessageBox>
#include <QPushButton>


class pop_up_window : public QMessageBox {
  Q_OBJECT
private:
  QAbstractButton *ok, *next;

public:
  pop_up_window(QWidget *parent = nullptr);
  pop_up_window(QWidget *parent, QString window_title, QString text,
                QPixmap pixmap);
  void pop_up();
  void set_button_text(QString, QString);
  void set_text(QString);
signals:
  void Ok();
  void Next();
};

#endif // POP_UP_WINDOW_H
