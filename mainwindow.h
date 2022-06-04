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
    float caculate(float e,float x,int o);
    void transfer_number();
    void clear();
    void update_operator_show();
    bool victory_judge();
    void help();
    bool victory_is_possible();

private:
    Ui::MainWindow *ui;
    int difficulty = 1;// 1,2,3,4
    int valid_operators_num = 4;
    int range[2];
    float expression = INT_MAX;
    int begin_with_number[4];
    QVector<QString> correct;
    QPushButton * numbers[5];
    QPushButton * operators[6];
    QAction * difficultes[4];
    int operation = 0;// 1 plus 2 minus 3 ... 6 multidivide
    bool show_max_cheet = false;

};
#endif // MAINWINDOW_H
