
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include "number.h"
#include "operation.h"
#include "calculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using i32 = int;
enum difficulties{
    easy,normal,hard,lunatic
};

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void new_game();
    i32 generate_numbers_by_difficulty();
    void change_difficulty(difficulties);
//    std::vector<operation>::iterator& find_operation_index(operations);
    bool change_operation(operations op, operation temp);

private:
    Ui::MainWindow *ui;
    std::vector<number> numbers;
    std::vector<operation> all_operation;
    difficulties difficulty = easy;
    QString cheat_info;
};

#endif // MAINWINDOW_H
