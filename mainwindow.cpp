#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BoardModel.h"
#include "boardDelegate.h"
#include "SudokuClass.h"
#include <QPushButton>
#include "RandomSudokuGenerator.h"
#include <QCursor>
#include <QMessageBox>
#include <QLabel>
#include <QTimer>
#include "HelperClass.h"
#include "resultsdialog.h"
#include <QRect>
#include <QApplication>
#include "newdialog.h"
#include <QDialog>
#include "about.h"
#include <QShortcut>
#include <QSignalMapper>
#include "sudokusettings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    delegate = NULL;
    activeSudoku = NULL;
    numberPick = NULL;
    numbersLeft = NULL;
    time = NULL;
    timer = NULL;

    numbersLeft = new QLabel(" Pozostało 99 pól. ");
    time = new QLabel(" Czas: 00:00:00 ");
    numbersLeft->setMinimumSize(numbersLeft->sizeHint());
    numbersLeft->setAlignment( Qt::AlignCenter );
    numbersLeft->setToolTip("Ilość niewypisanych pól na planszy");

    time->setMinimumSize( time->sizeHint() );
    time->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
    time->setToolTip("Czas gry");

    ui->statusBar->addPermanentWidget(numbersLeft);
    ui->statusBar->addPermanentWidget(time);
    ui->statusBar->repaint();

    QShortcut *shortcut = new QShortcut(this);
    shortcut->setKey(Qt::Key_Delete);
    shortcut->setContext(Qt::WindowShortcut);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteSlot()));

    std::vector<QShortcut *> * vec = new std::vector<QShortcut *>();
    QShortcut *shortcut1 = new QShortcut(this);
    shortcut1->setKey(Qt::Key_1);
    shortcut1->setContext(Qt::WindowShortcut);
    vec->push_back(shortcut1);
    QShortcut *shortcut2 = new QShortcut(this);
    shortcut2->setKey(Qt::Key_2);
    shortcut2->setContext(Qt::WindowShortcut);
    vec->push_back(shortcut2);
    QShortcut *shortcut3 = new QShortcut(this);
    shortcut3->setKey(Qt::Key_3);
    shortcut3->setContext(Qt::WindowShortcut);
    vec->push_back(shortcut3);
    QShortcut *shortcut4 = new QShortcut(this);
    shortcut4->setKey(Qt::Key_4);
    shortcut4->setContext(Qt::WindowShortcut);
    vec->push_back(shortcut4);
    QShortcut *shortcut5 = new QShortcut(this);
    shortcut5->setKey(Qt::Key_5);
    shortcut5->setContext(Qt::WindowShortcut);
    vec->push_back(shortcut5);
    QShortcut *shortcut6 = new QShortcut(this);
    shortcut6->setKey(Qt::Key_6);
    shortcut6->setContext(Qt::WindowShortcut);
    vec->push_back(shortcut6);
    QShortcut *shortcut7 = new QShortcut(this);
    shortcut7->setKey(Qt::Key_7);
    shortcut7->setContext(Qt::WindowShortcut);
    vec->push_back(shortcut7);
    QShortcut *shortcut8 = new QShortcut(this);
    shortcut8->setKey(Qt::Key_8);
    shortcut8->setContext(Qt::WindowShortcut);
    vec->push_back(shortcut8);
    QShortcut *shortcut9 = new QShortcut(this);
    shortcut9->setKey(Qt::Key_9);
    shortcut9->setContext(Qt::WindowShortcut);
    vec->push_back(shortcut9);

    QSignalMapper * signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(numberClickedSlot(int)));
    for(int i = 1; i < 10; i++){
        signalMapper->setMapping(vec->at(i-1), i);
        connect(vec->at(i-1), SIGNAL(activated()), signalMapper, SLOT(map()));
    }

    startNewSudoku();
}

MainWindow::~MainWindow()
{
    this->activeSudoku->clearMemory();
    delete this->activeSudoku;
    delete this->numberPick;
    delete ui;
}

void MainWindow::numberClicked(int num)
{
    if(this->settings->Numbers){
        this->delegate->getSudoku()->numberClicked(num);
        this->ui->boardView->viewport()->update();
    }
}

void MainWindow::updateNumbers()
{
    this->numbersLeft->setText(tr(" Pozostało %1 pól. ").arg(this->numbers[0]));
    for(int i = 1; i < 10; i++){
        QString name("num%1");
        name = name.arg(i);
        QPushButton* button = this->findChild<QPushButton*>(name);
        if(this->numbers[i] == 9){
            button->setText("");
            button->setEnabled(false);
        }else{
            button->setText(QString::number(i));
            button->setEnabled(true);
        }
    }
}

void MainWindow::updateTime()
{
    //co sekundę
    time->setText(this->timeToString());
}

void MainWindow::endIt()
{
    QCoreApplication::exit();
}

void MainWindow::startSudoku(SudokuStructure *newSudoku)
{
    activeSudoku = new SudokuClass(newSudoku, this->settings);
    BoardModel* model = new BoardModel();
    delegate = new BoardDelegate(model, activeSudoku);
    ui->boardView->setModel(model);
    ui->boardView->setItemDelegate(delegate);
    ui->boardView->setMouseTracking(true);
    int side = ui->boardView->columnWidth( 0 ) * 9 + 4;
    ui->boardView->resize( QSize( side, side ) );
    this->setFixedSize( QSize( side + 20, side + ui->statusBar->height() + ui->menuBar->height() + 42 ));
    ui->boardView->setEnabled( true );
    ui->boardView->clearSelection();
    ui->boardView->repaint();
    ui->menuBar->repaint();
    numbers = this->delegate->getSudoku()->updateNumberState();
    if(timer != NULL) delete timer;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);
    time->setText(" Czas: 00:00:00 ");
    this->seconds = 0;
    this->clickedCell = NULL;
    this->updateNumbers();
    ui->statusBar->repaint();
    this->ui->actionPodpowied->setEnabled(this->settings->Hints);
    if(this->isHidden())
        this->show();
}

void MainWindow::stopSudoku()
{
    int points = this->seconds;
    if(this->settings->ColsAndRows) points *= 1.2;
    if(this->settings->Errors) points *= 1.2;
    if(this->settings->Hints) points *= 1.2;
    if(this->settings->Numbers) points *= 1.2;
    ResultsDialog * rd = new ResultsDialog();
    this->timer->stop();
    delete this->timer;
    this->timer = NULL;
    rd->setResult(this->timeToString(),points);
    int result = rd->exec();
    if(result == 0) this->close();
    if(result == 1) this->startNewSudoku();
    delete rd;
}

void MainWindow::startNewSudoku()
{
    NewDialog * nd = new NewDialog();
    int result = nd->exec();
    this->settings = nd->getSettings();
    delete nd;
    switch(result)
    {
    case 1 : {

        //losowe sudoku
        RandomSudokuGenerator * rsg = new RandomSudokuGenerator();
        SudokuStructure * newSudoku = rsg->getRandomSudoku();
        delete rsg;
        this->startSudoku(newSudoku);
    }
        break;

    case 2 : {
        //wczytywanie sudoku z pliku
        SudokuStructure * newSudoku = HelperClass::loadSudokuFromFile();
        if(newSudoku != NULL)
            this->startSudoku(newSudoku);
        else
            this->startNewSudoku();
    }
        break;
    case 0 :
        //Wyłączenie okna
    {
        QTimer::singleShot(0, this, SLOT(endIt()));
    }
        break;
    }

}

QString MainWindow::timeToString()
{
    this->seconds++;
    long int duration = this->seconds;
    QString res;
    int seconds = (int) (duration % 60);
    duration /= 60;
    int minutes = (int) (duration % 60);
    duration /= 60;
    int hours = (int) (duration % 24);
    return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
}

void MainWindow::on_boardView_clicked(const QModelIndex &index)
{
    QPoint point = QCursor::pos();
    point.setX(point.x() - 251 / 2);
    point.setY(point.y() - 169 / 2);

    Cell * cell = this->delegate->getSudoku()->getCell(index.row()+1, index.column()+1);
    if(cell->getClicked() && !cell->getFixed()){
        if(this->numberPick == NULL){
            this->numberPick = new NumberPick(this);
        }
        this->numberPick->setCell(cell);
        this->numberPick->setNumbers(this->numbers);
        this->numberPick->move(point);
        this->numberPick->setWindowFlags(Qt::SplashScreen);
        this->numberPick->exec();
        this->delegate->getSudoku()->updateCellVal(index.row()+1, index.column()+1);
        this->ui->boardView->viewport()->update();
        this->numbers = this->delegate->getSudoku()->updateNumberState();
        this->updateNumbers();
        if(this->numbers[0] == 0 && this->delegate->getSudoku()->checkSudoku()){
            this->stopSudoku();
        }
    }else{
        this->delegate->getSudoku()->clickCell(index.row()+1, index.column()+1);
        this->ui->boardView->viewport()->update();
    }
    cell->setClicked(true);
    this->clickedCell = cell;
}

void MainWindow::on_num1_clicked()
{
    this->numberClicked(1);
}

void MainWindow::on_num2_clicked()
{
    this->numberClicked(2);
}

void MainWindow::on_num3_clicked()
{
    this->numberClicked(3);
}

void MainWindow::on_num4_clicked()
{
    this->numberClicked(4);
}

void MainWindow::on_num5_clicked()
{
    this->numberClicked(5);
}

void MainWindow::on_num6_clicked()
{
    this->numberClicked(6);
}

void MainWindow::on_num7_clicked()
{
    this->numberClicked(7);
}

void MainWindow::on_num8_clicked()
{
    this->numberClicked(8);
}

void MainWindow::on_num9_clicked()
{
    this->numberClicked(9);
}

void MainWindow::on_actionZako_cz_program_triggered()
{
    QCoreApplication::exit();
}

void MainWindow::on_actionPodpowied_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Podpowiedź.");
    msgBox.setInformativeText("Skorzystanie z podpowiedzi doda 30 sekund do twojego wyniku. Na pewno chcesz jej użyć?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(ret == QMessageBox::No) return;

    this->delegate->getSudoku()->getHint();
    this->seconds += 30;
    this->ui->boardView->viewport()->update();
    this->numbers = this->delegate->getSudoku()->updateNumberState();
    this->updateNumbers();
    if(this->numbers[0] == 0 && this->delegate->getSudoku()->checkSudoku()){
        this->stopSudoku();
    }
}

void MainWindow::on_actionZapisz_sudoku_do_pliku_triggered()
{
    //zapisz do pliku
    HelperClass::saveSudokuPuzzleToFile(this->delegate->getSudoku()->getSudokuStructure());
}

void MainWindow::on_actionO_aplikacji_triggered()
{
    //o aplikacji
    About about;
    about.exec();
}

void MainWindow::on_actionNowe_sudoku_triggered()
{
    this->timer->stop();
    delete this->timer;
    this->timer = NULL;
    this->hide();
    this->startNewSudoku();
}

void MainWindow::deleteSlot()
{
    if(this->clickedCell != NULL){
        if(!this->clickedCell->getFixed()){
            this->clickedCell->setVal(0);
            this->delegate->getSudoku()->updateCellVal(this->clickedCell->getRow(), this->clickedCell->getCol());
            this->ui->boardView->viewport()->update();
            this->numbers = this->delegate->getSudoku()->updateNumberState();
            this->updateNumbers();
            if(this->numbers[0] == 0 && this->delegate->getSudoku()->checkSudoku())
                this->stopSudoku();
        }
    }
}

void MainWindow::numberClickedSlot(int number)
{
    if(this->clickedCell != NULL){
        if(!this->clickedCell->getFixed()){
            this->clickedCell->setVal(number);
            this->delegate->getSudoku()->updateCellVal(this->clickedCell->getRow(), this->clickedCell->getCol());
            this->ui->boardView->viewport()->update();
            this->numbers = this->delegate->getSudoku()->updateNumberState();
            this->updateNumbers();
            if(this->numbers[0] == 0 && this->delegate->getSudoku()->checkSudoku())
                this->stopSudoku();
        }
    }
}
