#include "newdialog.h"
#include "ui_newdialog.h"
#include <QRect>
#include <QSize>
#include <QDesktopWidget>
#include <QCloseEvent>
#include "sudokusettings.h"

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDialog)
{
    ui->setupUi(this);
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
    this->setFixedSize( QSize( this->width(), this->height()));
}

SudokuSettings *NewDialog::getSettings()
{
    SudokuSettings * settings = new SudokuSettings();
    settings->ColsAndRows = this->ui->cbColsRows->isChecked();
    settings->Errors = this->ui->cbErrors->isChecked();
    settings->Hints = this->ui->cbHints->isChecked();
    settings->Numbers = this->ui->cbNumbers->isChecked();
    return settings;
}

NewDialog::~NewDialog()
{
    delete ui;
}

void NewDialog::on_commandLinkButton_clicked()
{
    this->done(1);
}

void NewDialog::on_commandLinkButton_2_clicked()
{
    //wczytywanie z pliku
    this->done(2);
}
