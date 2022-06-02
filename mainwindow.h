#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void newgame();
    void update_valid_operator();
    void update_expression();
    void number_clicked(QPushButton * button);
    void activate_buttons();
    void caculate(float x);
    void transfer_number();
    void clear();
    void update_operator_show();
    bool victory_pd();
    void help();
//    void get_operation(QPushButton *button);

private:
    Ui::MainWindow *ui;
    int difficulty = 1;// 1,2,3,4
    int valid_operators_num = 4;
    int range[2];
    float expression = INT_MAX;
    QPushButton * numbers[5];
    QPushButton * operators[6];
    QAction * difficultes[4];
    int operation = 0;// 1 plus 2 minus 3 ... 6 multidivide


};
#endif // MAINWINDOW_H
