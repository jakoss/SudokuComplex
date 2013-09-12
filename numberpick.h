#ifndef NUMBERPICK_H
#define NUMBERPICK_H

#include <QDialog>
#include "Cell.h"

namespace Ui {
class NumberPick;
}

class NumberPick : public QDialog
{
    Q_OBJECT
    
public:
    explicit NumberPick(QWidget *parent = 0);
    ~NumberPick();
    void setCell(Cell * cell);
    void setNumbers(std::vector<int> numbers);
    
private slots:
    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_1_clicked();

    void on_cbHint_clicked();

private:
    Ui::NumberPick *ui;
    Cell * cell;
    void pickNumber(int number);
};

#endif // NUMBERPICK_H
