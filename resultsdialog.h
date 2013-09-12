#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class ResultsDialog;
}

class ResultsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ResultsDialog(QWidget *parent = 0);
    ~ResultsDialog();
    void setResult(QString time, int points);
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ResultsDialog *ui;
};

#endif // RESULTSDIALOG_H
