#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QMessageBox>

int randint(int,int);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("24pointsgame made by |x|");
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
            difficulty = i + 1;
            newgame();
        });
    }
    connect(ui->transfer,&QPushButton::clicked,this,&MainWindow::transfer_number);
    connect(ui->clear,&QPushButton::clicked,this,&MainWindow::clear);
    connect(ui->actionnew_game,&QAction::triggered,this,&MainWindow::newgame);
    connect(ui->actionhelp,&QAction::triggered,this,&MainWindow::help);
    newgame();
}

void MainWindow::newgame()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    range[1] = 10;
    if(difficulty % 2 == 0)
        range[0] = -10;
    else range[0] = 1;
    update_valid_operator();
    expression = INT_MAX;
    operation = 0;
    activate_buttons();
    update_expression();

    for(int i = 0;i < 4;++i)
    {
        numbers[i]->setText(QString::number(randint(range[0],range[1])));
    }
}

void MainWindow::number_clicked(QPushButton *button)
{
    if(button->text().isEmpty()) return;
    if (expression == INT_MAX)
    {
        expression = button->text().toInt();
    }
    else if(operation)
    {
        caculate(button->text().toFloat());
        operation = 0;
        update_operator_show();
    }
    else return;
    update_expression();
    button->setDisabled(true);
    if (button == numbers[4])
        button->setText(QString());

    if(victory_pd())
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

void MainWindow::caculate(float x)
{
    switch (operation) {
        case 1:expression += x;break;
        case 2:expression -= x;break;
        case 3:expression *= x;break;
        case 4:expression /= x;break;
        case 5:expression = powf(expression,x);break;
        case 6:expression = (int)expression / (int) x;break;
        default:break;
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
}

bool MainWindow::victory_pd()
{
    if(expression != 24.0) return false;
    for (int i = 0;i < 5;++i) {
        if (numbers[i]->isEnabled()){
            return false;
        }
    }
    return true;
}

void MainWindow::help()
{
    QString rule = QString("You should use all 4 numbers to calculate out 24.\n"
                           "'^' means pow, and '//' means divide then floor the result.\n"
                           "Decimal or negative numbers is available.\n"
                           "Enjoy yourself!");
    QMessageBox::about(this,QString("help: 24points game"),rule);
}
