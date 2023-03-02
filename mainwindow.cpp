#include <QTime>
#include <QDebug>
#include <ranges>
#include <stdexcept>
#include <QThread>
#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("super24points");
    setWindowIcon(QIcon(":/static/logo.ico"));

    all_operation = std::vector<operation>{
        operation(operations::add,[](i32 x,i32 y){return x + y;},ui->b1),
        operation(operations::minus,[](i32 x,i32 y){return x - y;},ui->b2),
        operation(operations::times,[](i32 x,i32 y){return x * y;},ui->b3),
        operation(operations::divide,[](i32 x,i32 y){return x / y;},ui->b4),
        operation(operations::xor_,[](i32 x,i32 y){return x^y;},ui->b5),
//        operation(operations::power,[](i32 x,i32 y){return pow(x,y);},ui->b5),
        operation(operations::and_,[](i32 x,i32 y){return x&y;},ui->b6),
        operation(operations::or_,[](i32 x,i32 y){return x|y;},ui->b7),
        operation(operations::divide_exactly,[](i32 x,i32 y){
                return static_cast<i32>(static_cast<double>(x) / static_cast<double>(y));
            },ui->b8)
    };
    for (auto &i : all_operation)
    {
        map_all_operation.insert({i.get_operation(),i});
    }
    map_all_operation.insert({operations::power,operation(operations::power,[](i32 x,i32 y){return pow(x,y);},ui->b5)});
    map_difficulty_actions = decltype(map_difficulty_actions){
        {difficulties::easy,ui->actioneasy},
        {difficulties::normal,ui->actionnormal},
        {difficulties::hard,ui->actionhard},
        {difficulties::lunatic,ui->actionlunatic},
    };
    messagebox_init();
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
        QMessageBox::about(this,QString("cheat"),calculator::calculate(numbers));
    });
    connect(ui->clear,&QPushButton::clicked,this,&MainWindow::clear);
    for (auto &i : all_operation)
    {
        connect(i.get_button(),&QPushButton::clicked,this,[&](){
            if (!expression.has_value()) return;
            symbol = i;
            print();
        });
    }
    connect(ui->transfer,&QPushButton::clicked,this,&MainWindow::move_into_register);
    connect(ui->temp,&QPushButton::clicked,this,[this](){
        move_into_expression(register_.value());
        register_ = std::nullopt;
    });
    new_game();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::new_game()
{
    if (register_.has_value()) register_->use();
    register_ = std::nullopt;
    expression = std::nullopt;
    symbol = std::nullopt;
    std::ranges::for_each(numbers,[](number &i){
        i.get_button()->disconnect();
        i.clear();
    });
    numbers.clear();
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num1));
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num2));
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num3));
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num4));
    std::ranges::for_each(numbers,[this](number &i){
        i.get_button()->connect(i.get_button(),&QPushButton::clicked,this,[&](){
            move_into_expression(i);
        });
    });
    deal_operators_by_difficulty();
    print();
}

void MainWindow::clear()
{
    std::ranges::for_each(numbers,[](number &i){i.clear();});
    if (register_.has_value()) register_->use();
    register_ = std::nullopt;
    expression = std::nullopt;
    symbol = std::nullopt;
    print();
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

void MainWindow::deal_operators_by_difficulty()
{
    switch(difficulty)
    {
    case hard:
        for (auto &i : all_operation){
            if (i.get_operation() == operations::xor_ || i.get_operation() == operations::and_ || i.get_operation() == operations::or_)
                i.set_activated(false);
            else i.set_activated(true);
        }
        break;
    case lunatic:
        for (auto &i : all_operation){
            if (i.get_operation() == operations::power || i.get_operation() == operations::divide_exactly)
                i.set_activated(false);
            else i.set_activated(true);
        }
        break;
    default:
        for (auto &i : all_operation){
            if (i.get_operation() == operations::power || i.get_operation() == operations::divide_exactly ||
            i.get_operation() == operations::xor_ || i.get_operation() == operations::and_ || i.get_operation() == operations::or_)
                i.set_activated(false);
            else i.set_activated(true);
        }
        break;
    }
}

void MainWindow::change_difficulty(difficulties temp)
{
    difficulty = temp;
    for (auto &i : map_difficulty_actions)
    {
        i.second->setChecked(false);
    }
    map_difficulty_actions.at(temp)->setChecked(true);
    // exchange the meaning of "^"
    switch(temp)
    {
    case hard:
        change_operation(operations::xor_,map_all_operation.at(operations::power));
        break;
    default:
        change_operation(operations::power,map_all_operation.at(operations::xor_));
        break;
    }
    deal_operators_by_difficulty();
}

void MainWindow::print()
{
    if (expression.has_value())
        ui->expression->setText(QString::number(expression.value()));
    else ui->expression->setText(QString());
    if (symbol.has_value())
        ui->operator_show->setText(symbol.value().get_op_string());
    else ui->operator_show->setText(QString());
}

bool MainWindow::move_into_register()
{
    if (!expression.has_value())
        return false;
    if (symbol.has_value()) symbol = std::nullopt;
    register_ = number(expression.value(),ui->temp);
    expression = std::nullopt;
    print();
    return true;
}

bool MainWindow::move_into_expression(number& num)
{
    if (symbol.has_value()) // symbol has value means expression has value
    {
        try{
            if (num.get_num() == 0)
                throw QString("Division by zero condition!");
            expression = symbol.value().func(expression.value(),num.get_num());
            symbol = std::nullopt;
        } catch (QString err) {
            QMessageBox::about(this,"err",err);
            clear();
        }
    }
    else if (expression.has_value())
        return false;
    else
        expression = num.get_num();
    num.use();
    print();
    victory_check();
    return true;
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

bool MainWindow::is_win()
{
    return expression.has_value() && expression.value() == 24 && !ui->num1->isEnabled() && !ui->num2->isEnabled()
           && !ui->num3->isEnabled() && !ui->num4->isEnabled() && !register_.has_value();
}

void MainWindow::victory_check()
{
    if (!is_win()) return;
    box_win.exec();
    if (box_win.clickedButton() == box_win_next)
    {
        new_game();
    }
}

void MainWindow::messagebox_init()
{
    box_win.setParent(this);
    box_win.setWindowFlags(Qt::Dialog);
    box_win.setWindowTitle("Congratulations");
    box_win.setText("YOU WIN!");
    box_win.setIconPixmap(QPixmap(":/static/win.ico"));//.scaled(200,200,Qt::KeepAspectRatio)
    box_win.setWindowIcon(QIcon(":/static/win.ico"));
    box_win_ok = box_win.addButton("close",QMessageBox::RejectRole);
    box_win_next = box_win.addButton("new game",QMessageBox::YesRole);
}
