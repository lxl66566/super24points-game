#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QMessageBox>
#include <algorithm>

int randint(int,int);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("24pointsgame made by |x|");
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    numbers[0] = ui->num1;
    numbers[1] = ui->num2;
    numbers[2] = ui->num3;
    numbers[3] = ui->num4;
    numbers[4] = ui->temp;
    operators[0] = ui->plus;
    operators[1] = ui->minus;
    operators[2] = ui->multiply;
    operators[3] = ui->divide;
    operators[4] = ui->pow;
    operators[5] = ui->multidivide;
    difficultes[0] = ui->actioneasy;
    difficultes[1] = ui->actionnormal;
    difficultes[2] = ui->actionhard;
    difficultes[3] = ui->actionlunatic;

    for (int i = 0;i < 5;++i) {
        connect(numbers[i],&QPushButton::clicked,this,[=](){number_clicked(numbers[i]);});
    }
    for (int i = 0;i < 6;++i) {
        connect(operators[i],&QPushButton::clicked,this,[=](){
            operation = i + 1;
            update_operator_show();
        });
    }
    for (int i = 0;i < 4 ;++i)
    {
        connect(difficultes[i],&QAction::triggered,this,[=](){
            for (int j = 0;j < 4;++j)
                difficultes[j]->setChecked(false);
            difficultes[i]->setChecked(true);
            difficulty = i + 1;
            newgame();
        });
    }
    connect(ui->transfer,&QPushButton::clicked,this,&MainWindow::transfer_number);
    connect(ui->clear,&QPushButton::clicked,this,&MainWindow::clear);
    connect(ui->actionnew_game,&QAction::triggered,this,&MainWindow::newgame);
    connect(ui->actionhelp,&QAction::triggered,this,&MainWindow::help);
    connect(ui->actioncheat,&QAction::triggered,this,[=]()
    {
        QString answer = QString();
        int temp = 0;
        for (QVector<QString>::iterator i = correct.begin();i != correct.end();++i,++temp)
        {
            if (!(temp % 2))
            {
                answer += QString("%1").arg(*i,-30);
            }
            else
            {
                answer += QString("%1").arg(*i,20);
                answer += QString("\n");
            }
        }
        QMessageBox::about(this,QString("cheat"),answer);
    });
    connect(ui->actionshow_max_cheat,&QAction::triggered,this,[=](){
        show_max_cheet = not show_max_cheet;
        ui->actionshow_max_cheat->setChecked(show_max_cheet);
        victory_is_possible();
    });
    newgame();
}

void MainWindow::newgame()
{
    range[1] = 10;
    if(difficulty % 2 == 0)
        range[0] = -10;
    else range[0] = 1;
    update_valid_operator();
    clear();

    do
    {
        for (int i = 0;i < 4;++i) {
            begin_with_number[i] = randint(range[0],range[1]);
        }
    }
    while (!victory_is_possible());
    for(int i = 0;i < 4;++i)
    {
        numbers[i]->setText(QString::number(begin_with_number[i]));
    }
    ui->actionshow_max_cheat->setChecked(show_max_cheet);
}

void MainWindow::number_clicked(QPushButton *button)
{
    if(button->text().isEmpty()) return;
    if (expression == INT_MAX)
    {
        expression = button->text().toFloat();
    }
    else if(operation)
    {
        expression = caculate(expression,button->text().toFloat(),operation);
        operation = 0;
        update_operator_show();
    }
    else return;
    update_expression();
    button->setDisabled(true);
    if (button == numbers[4])
        button->setText(QString());

    if(victory_judge())
    {
        QMessageBox::about(this,QString(),QString("YOU WIN!!"));
        return;
    }
}

void MainWindow::transfer_number()
{
    if (expression == INT_MAX or !numbers[4]->text().isEmpty()) return;
    numbers[4]->setText(QString::number(expression));
    expression = INT_MAX;
    numbers[4]->setEnabled(true);
    update_expression();
}

float MainWindow::caculate(float e,float x,int o)
{
    switch (o) {
        case 1:return e + x;
        case 2:return e - x;
        case 3:return e * x;
        case 4:return x ? e / x : INT_MAX;
        case 5:return powf(e,x);
        case 6:return x ? floor(e / x) : INT_MAX;
        default:return INT_MAX;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

int randint(int x,int y)
{
    int temp = qrand() % (y - x + 1) + x;
    while(!temp)
        temp = qrand() % (y - x + 1) + x;
    return temp;
}

void MainWindow::update_valid_operator()
{
    valid_operators_num = difficulty <= 2 ? 4 : 6;
    if(difficulty <= 2)
    {
        operators[4]->hide();
        operators[5]->hide();
    }
    else{
        operators[4]->show();
        operators[5]->show();
    }
}

void MainWindow::update_expression()
{
    if (expression == INT_MAX)
    {
        ui->expression->setText(QString());
        return;
    }
    ui->expression->setText(QString::number(expression));
}

void MainWindow::update_operator_show()
{
    if (!operation)
        ui->operator_show->setText(QString());
    else
        ui->operator_show->setText(operators[operation - 1]->text());
}

void MainWindow::activate_buttons()
{
    for (int i = 0;i < 5;++i) {
        numbers[i]->setEnabled(true);
    }
    for (int i = 0;i < 6;++i) {
        operators[i]->setEnabled(true);
    }
    ui->temp->setDisabled(true);
}

void MainWindow::clear()
{
    expression = INT_MAX;
    operation = 0;
    activate_buttons();
    update_expression();
    numbers[4]->setText(QString());
    ui->operator_show->setText(QString());
}

bool MainWindow::victory_judge()
{
    if(expression != 24.0) return false;
    for (int i = 0;i < 5;++i) {
        if (numbers[i]->isEnabled()){
            return false;
        }
    }
    return true;
}

bool MainWindow::victory_is_possible()
{
    bool flag = false;
    int num[4];
    for (int i = 0;i < 4; ++i) num[i] = begin_with_number[i];
    std::sort(num,num + 4);
    do
    {
        for (int o1 = 1; o1 <= valid_operators_num;++o1)
            for (int o2 = 1; o2 <= valid_operators_num;++o2)
                for (int o3 = 1; o3 <= valid_operators_num;++o3)
                {
                    if (caculate(caculate(caculate(num[0],num[1],o1),num[2],o2),num[3],o3) == 24.0)
                    {
                        if (!flag){
                            flag = true;
                            correct.clear();
                            correct.squeeze();
                        }
                        correct.push_back(
                                QString("((%1 %2 %3) %4 %5) %6 %7").arg(num[0]).arg(operators[o1 - 1]->text()).arg(num[1])
                                .arg(operators[o2 - 1]->text()).arg(num[2]).arg(operators[o3 - 1]->text()).arg(num[3])
                                );
                        if (!show_max_cheet)
                            return flag;
                    }
                    if (caculate(caculate(num[0],num[1],o1),caculate(num[2],num[3],o2),o3) == 24.0)
                    {
                        if (!flag){
                            flag = true;
                            correct.clear();
                            correct.squeeze();
                        }
                        correct.push_back(
                                    QString("(%1 %2 %3) %4 (%5 %6 %7)").arg(num[0]).arg(operators[o1 - 1]->text()).arg(num[1])
                                .arg(operators[o3 - 1]->text()).arg(num[2]).arg(operators[o2 - 1]->text()).arg(num[3])
                                );
                        if (!show_max_cheet)
                            return flag;
                    }
                }

    }
    while(std::next_permutation(num , num + 4));
    return flag;
}

void MainWindow::help()
{
    QString rule = QString("You should use all 4 numbers to calculate out 24.\n"
                           "'^' means pow, and '//' means divide then floor the result.\n"
                           "'<--' button can push result number into register.\n"
                           "'show max cheat' mode could show every possible answers in cheat.\n"
                           "Decimal or negative numbers is available.\n"
                           "Enjoy yourself!");
    QMessageBox::about(this,QString("help: 24points game"),rule);
}
