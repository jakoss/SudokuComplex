#include "resultsdialog.h"
#include "ui_resultsdialog.h"
#include <QDesktopWidget>
#include <QRect>
#include <QSize>
#include <QCloseEvent>

ResultsDialog::ResultsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultsDialog)
{
    ui->setupUi(this);
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
    this->setFixedSize( QSize( this->width(), this->height()));
}

ResultsDialog::~ResultsDialog()
{
    delete ui;
}

void ResultsDialog::setResult(QString time, int points)
{
    this->ui->lblPoints->setText(QString::number(points));
    this->ui->lblTime->setText(time);
}

void ResultsDialog::on_pushButton_clicked()
{
    QCoreApplication::exit();
}

void ResultsDialog::on_pushButton_2_clicked()
{
    this->done(1);
}
