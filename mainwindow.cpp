// #include <QTime>
#include <QDebug>
#include <ranges>
// #include <stdexcept>
#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("super%1points").arg(TARGET_NUMBER));
    setWindowIcon(QIcon(":/static/logo.ico"));

    all_operation = std::vector<operation>{
        operation(operations::add,[](f64 x,f64 y){return x + y;},ui->b1),
        operation(operations::minus,[](f64 x,f64 y){return x - y;},ui->b2),
        operation(operations::times,[](f64 x,f64 y){return x * y;},ui->b3),
        operation(operations::divide,[](f64 x,f64 y){return y == 0 ? INT_MAX : x / y;},ui->b4),
        operation(operations::xor_,[](f64 x,f64 y){return static_cast<i32>(x)^static_cast<i32>(y);},ui->b5),
//        operation(operations::power,[](f64 x,f64 y){return pow(x,y);},ui->b5),
        operation(operations::and_,[](f64 x,f64 y){return static_cast<i32>(x)&static_cast<i32>(y);},ui->b6),
        operation(operations::or_,[](f64 x,f64 y){return static_cast<i32>(x)|static_cast<i32>(y);},ui->b7),
        operation(operations::divide_exactly,[](f64 x,f64 y){
                return y == 0 ? INT_MAX : static_cast<int>(x / y);
            },ui->b8)
    };
    for (auto &i : all_operation)
    {
        map_all_operation.insert({i.get_operation(),i});
    }
    map_all_operation.insert({operations::power,operation(operations::power,[](f64 x,f64 y){return pow(x,y);},ui->b5)});
    qDebug() << "operations initialized.";
    map_difficulty_actions = decltype(map_difficulty_actions){
        {difficulties::easy,ui->actioneasy},
        {difficulties::normal,ui->actionnormal},
        {difficulties::hard,ui->actionhard},
        {difficulties::lunatic,ui->actionlunatic},
    };
    messagebox_init();
    qDebug() << "messagebox set.";
    connect(ui->actionnew_game,&QAction::triggered,this,&MainWindow::new_game);
    connect(ui->actionhelp,&QAction::triggered,this,[this](){
        QMessageBox::about(this,QString("help: %1points game").arg(TARGET_NUMBER),QString(
R""(You should use all 4 numbers to calculate out %1.
'^' means pow, and '//' means divide then floor the result.
'<--' button can push result number into register.
'show max cheat' mode could show every possible answers in cheat.
Decimal or negative numbers is available.
Enjoy yourself!)""
).arg(TARGET_NUMBER));
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
        QMessageBox::about(this,QString("cheat"),calculator_.get_ans());
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
        victory_check();
    });
    qDebug() << "connect completed.";
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
    for(auto &i : numbers){
        i.clear();
        i.get_button()->disconnect();
    }
    numbers.clear();
    qDebug() << "old numbers clear.";
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num1));
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num2));
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num3));
    numbers.push_back(number(generate_numbers_by_difficulty(),ui->num4));
    qDebug() << "numbers set.";
    for (auto &i : numbers){
        qDebug() << "numberinfo: " + i.get_string_num() + i.get_button()->text();
        i.get_button()->connect(i.get_button(),&QPushButton::clicked,this,[&i,this](){
            move_into_expression(i);
        });
    }
    qDebug() << "numbers connected complete.";
    deal_operators_by_difficulty();
    qDebug() << "hide & show operators completed.";
    qDebug() << "entered calculator";
    calculator_ = calculator(numbers,all_operation);
    qDebug() << "finished calculator";
    if (calculator_.get_ans().isEmpty())
    {
        qDebug() << "there's no answer, regenerating..";
        new_game();
        return;
    }
    print();
}

void MainWindow::clear()
{
    if (register_.has_value()) register_->use();
    std::ranges::for_each(numbers,[](number &i){i.clear();});
//    for (auto &i : numbers) i.clear();  // i suspended that std::ranges::for_each has bugs with my program...
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
        auto temp = symbol.value().dynamic_func(expression.value(),num.get_num());
        try{
//            if (num.get_num() == 0
//                && (symbol.value().get_operation() == operations::divide
//                    || symbol.value().get_operation() == operations::divide_exactly))
//                throw QString("Division by zero condition!");
//            if ((!num.is_i32() || !number::is_i32(expression.value())) && !symbol.value().accept_f64_calculation())
//                throw QString("Cannot do this operation to a float number!");
            expression = std::get<f64>(temp);
            symbol = std::nullopt;
        } catch (std::bad_variant_access&) {
            QString err = std::get<QString>(temp);
            window_error->set_text(err);
            window_error->pop_up();
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
    auto it = std::find_if(all_operation.begin(),all_operation.end(),[&op](const operation &i){return i.get_operation() == op;});
    if (it == all_operation.end()) return false;
    *it = temp;
    return true;
}

bool MainWindow::is_win()
{
    return expression.has_value() && expression.value() == TARGET_NUMBER && !ui->num1->isEnabled() && !ui->num2->isEnabled()
           && !ui->num3->isEnabled() && !ui->num4->isEnabled() && !register_.has_value();
}

void MainWindow::victory_check()
{
    if (!is_win()) return;
    window_win->pop_up();
}

void MainWindow::messagebox_init()
{
    window_win = std::make_unique<pop_up_window>(this,"Congratulations","YOU WIN!",QPixmap(":/static/win.ico"));
    connect(window_win.get(),&pop_up_window::Next,this,[this](){new_game();});
    window_error = std::make_unique<pop_up_window>(this,"error","Division by zero condition!",QPixmap(":/static/err.ico"));
    window_error->set_button_text("OK","Sorry");
}
