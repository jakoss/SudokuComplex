#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include "sudokusettings.h"

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewDialog(QWidget *parent = 0);
    SudokuSettings * getSettings();
    ~NewDialog();
    
private slots:
    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();

private:
    Ui::NewDialog *ui;
};

#endif // NEWDIALOG_H
