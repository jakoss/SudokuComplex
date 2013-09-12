#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BoardDelegate.h"
#include "numberpick.h"
#include <QLabel>
#include <QString>
#include <QShortcut>
#include "sudokusettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;

private:
    BoardDelegate *delegate;
    SudokuClass *activeSudoku;
    NumberPick *numberPick;
    QLabel *numbersLeft;
    QLabel *time;
    QTimer *timer;
    Cell *clickedCell;
    SudokuSettings * settings;
    std::vector<int> numbers;
    long int seconds;

    void numberClicked(int num);
    void updateNumbers();
    void startSudoku(SudokuStructure * newSudoku);
    void stopSudoku();
    void startNewSudoku();
    QString timeToString();

private slots:
    void on_boardView_clicked(const QModelIndex &index);
    void on_num1_clicked();
    void on_num2_clicked();
    void on_num3_clicked();
    void on_num4_clicked();
    void on_num5_clicked();
    void on_num6_clicked();
    void on_num7_clicked();
    void on_num8_clicked();
    void on_num9_clicked();
    void updateTime();
    void endIt();
    void on_actionZako_cz_program_triggered();
    void on_actionPodpowied_triggered();
    void on_actionZapisz_sudoku_do_pliku_triggered();
    void on_actionO_aplikacji_triggered();
    void on_actionNowe_sudoku_triggered();
    void deleteSlot();
    void numberClickedSlot(int number);
};

#endif // MAINWINDOW_H
