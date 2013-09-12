#include "numberpick.h"
#include "ui_numberpick.h"

NumberPick::NumberPick(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumberPick)
{
    ui->setupUi(this);
}

NumberPick::~NumberPick()
{
    delete ui;
}

void NumberPick::setCell(Cell *cell)
{
    this->cell = cell;
    if(this->ui->cbHint->isChecked()){
        for(int i = 0; i < 9; i++){
            QString name("pushButton_%1");
            name = name.arg(i+1);
            QPushButton* button = this->findChild<QPushButton*>(name);
            if(this->cell->isHint(i)){
                QFont newFont = button->font();
                newFont.setBold(true);
                button->setFont(newFont);
            }else{
                QFont newFont = button->font();
                newFont.setBold(false);
                button->setFont(newFont);
            }
        }
    }
}

void NumberPick::setNumbers(std::vector<int> numbers)
{
    for(int i = 1; i < 10; i++){
        QString name("pushButton_%1");
        name = name.arg(i);
        QPushButton* button = this->findChild<QPushButton*>(name);
        if(numbers[i] == 9){
            button->setEnabled(false);
        }else{
            button->setEnabled(true);
        }
    }
}

void NumberPick::on_pushButton_11_clicked()
{
    //przycisk "zatwierdź"
    this->done(1);
}

void NumberPick::on_pushButton_10_clicked()
{
    //przycisk "wyzeruj pole"
    this->cell->setVal(0);
    this->done(1);
}

void NumberPick::pickNumber(int number)
{
    //wybrany numer
    if(this->ui->cbHint->isChecked()){
        QString name("pushButton_%1");
        name = name.arg(number);
        QPushButton* button = this->findChild<QPushButton*>(name);
        if(this->cell->isHint(number-1)){
            QFont newFont = button->font();
            newFont.setBold(false);
            button->setFont(newFont);
            this->cell->removeHint(number-1);
        }else{
            QFont newFont = button->font();
            newFont.setBold(true);
            button->setFont(newFont);
            this->cell->addHint(number-1);
        }
    }else{
        this->cell->setVal(number);
        this->done(1);
    }
}

void NumberPick::on_pushButton_1_clicked()
{
    this->pickNumber(1);
}

void NumberPick::on_pushButton_2_clicked()
{
    this->pickNumber(2);
}

void NumberPick::on_pushButton_3_clicked()
{
    this->pickNumber(3);
}

void NumberPick::on_pushButton_4_clicked()
{
    this->pickNumber(4);
}

void NumberPick::on_pushButton_5_clicked()
{
    this->pickNumber(5);
}

void NumberPick::on_pushButton_6_clicked()
{
    this->pickNumber(6);
}

void NumberPick::on_pushButton_7_clicked()
{
    this->pickNumber(7);
}

void NumberPick::on_pushButton_8_clicked()
{
    this->pickNumber(8);
}

void NumberPick::on_pushButton_9_clicked()
{
    this->pickNumber(9);
}

void NumberPick::on_cbHint_clicked()
{
    if(this->ui->cbHint->isChecked()){
        for(int i = 0; i < 9; i++){
            QString name("pushButton_%1");
            name = name.arg(i+1);
            QPushButton* button = this->findChild<QPushButton*>(name);
            if(this->cell->isHint(i)){
                QFont newFont = button->font();
                newFont.setBold(true);
                button->setFont(newFont);
            }else{
                QFont newFont = button->font();
                newFont.setBold(false);
                button->setFont(newFont);
            }
        }
    }else{
        for(int i = 0; i < 9; i++){
            QString name("pushButton_%1");
            name = name.arg(i+1);
            QPushButton* button = this->findChild<QPushButton*>(name);
            QFont newFont = button->font();
            newFont.setBold(false);
            button->setFont(newFont);
        }
    }
}
