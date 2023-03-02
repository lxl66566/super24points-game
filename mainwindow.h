
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <QMessageBox>
#include "number.h"
#include "operation.h"
#include "calculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using i32 = int;
using f64 = double;
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
    void clear();
    i32 generate_numbers_by_difficulty();
    void deal_operators_by_difficulty();
    void change_difficulty(difficulties);
    void print();
    bool move_into_register();
    bool move_into_expression(number&);
//    std::vector<operation>::iterator& find_operation_index(operations);
    bool change_operation(operations op, operation temp);
    bool is_win();
    void victory_check();
    void messagebox_init();

private:
    Ui::MainWindow *ui;
    std::vector<number> numbers;
    std::vector<operation> all_operation;
    difficulties difficulty = easy;
    std::unordered_map<difficulties,QAction*> map_difficulty_actions;
    std::unordered_map<operations,operation> map_all_operation;
    QString cheat_info;
    std::optional<i32> expression;
    std::optional<operation> symbol;
    std::optional<number> register_;
    QMessageBox box_win;
    QAbstractButton *box_win_ok,*box_win_next;
};

#endif // MAINWINDOW_H
