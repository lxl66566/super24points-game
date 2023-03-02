#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    all_operation = std::vector<operation>{
        operation(operations::add,[](i32 x,i32 y){return x + y;},ui->b1),
        operation(operations::minus,[](i32 x,i32 y){return x - y;},ui->b2),
        operation(operations::times,[](i32 x,i32 y){return x * y;},ui->b3),
        operation(operations::divide,[](i32 x,i32 y){return x / y;},ui->b4),
        operation(operations::xor_,[](i32 x,i32 y){return x^y;},ui->b5),
        operation(operations::and_,[](i32 x,i32 y){return x&y;},ui->b6),
        operation(operations::or_,[](i32 x,i32 y){return x|y;},ui->b7),
        operation(operations::divide_exactly,[](i32 x,i32 y){
                return static_cast<i32>(static_cast<double>(x) / static_cast<double>(y));
            },ui->b8)
    };
    connect(ui->actionnew_game,&QAction::triggered,this,&MainWindow::new_game);
    connect(ui->actionhelp,&QAction::triggered,this,[this](){
        QMessageBox::about(this,QString("help: 24points game"),QString(
R""(You should use all 4 numbers to calculate out 24.
'^' means pow, and '//' means divide then floor the result.
'<--' button can push result number into register.
'show max cheat' mode could show every possible answers in cheat.
Decimal or negative numbers is available.
Enjoy yourself!)""
));
    });
    connect(ui->actioneasy,&QAction::triggered,this,[this](){
        change_difficulty(easy);
        new_game();
    });
    connect(ui->actionnormal,&QAction::triggered,this,[this](){
        change_difficulty(normal);
        new_game();
    });
    connect(ui->actionhard,&QAction::triggered,this,[this](){
        change_difficulty(hard);
        new_game();
    });
    connect(ui->actionlunatic,&QAction::triggered,this,[this](){
        change_difficulty(lunatic);
        new_game();
    });
    connect(ui->actioncheat,&QAction::triggered,this,[this](){
        std::vector<i32> temp(4);
        for (auto i : numbers) temp.push_back(i.get_num());
        QMessageBox::about(this,QString("cheat"),calculator::calculate(temp));
    });
    new_game();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::new_game()
{
    for (auto &i : numbers)
    {
        i.clear();
    }
    numbers.clear();
    all_operation.clear();
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num1));
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num2));
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num3));
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num4));

}

i32 MainWindow::generate_numbers_by_difficulty()
{
    switch(difficulty)
    {
    case normal:{
        i32 temp = QRandomGenerator::global()->bounded(-10,11);   // [-10,10] except 0
        while(temp == 0) temp = QRandomGenerator::global()->bounded(-10,11);
        return temp;
    }
    default:
        return QRandomGenerator::global()->bounded(1,11);
    }
}

void MainWindow::change_difficulty(difficulties temp)
{
    difficulty = temp;
    switch(temp)
    {
    case hard:
        change_operation(operations::xor_,operation(operations::power,[](i32 x,i32 y){return pow(x,y);},ui->b5));
        break;
    default:
        change_operation(operations::power,operation(operations::xor_,[](i32 x,i32 y){return x^y;},ui->b5));
        break;
    }
}


bool MainWindow::change_operation(operations op, operation temp)
{
    for (auto i = all_operation.begin();i != all_operation.end();++i)
    {
        if ((*i).get_operation() == op)
        {
            *i = temp;
            return true;
        }
    }
    return false;
}
