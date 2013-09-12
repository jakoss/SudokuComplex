#include "SudokuClass.h"
#include <QColor>
#include <time.h>
#include "Types.h"

SudokuClass::SudokuClass(SudokuStructure * sudoku, SudokuSettings *settings)
{
    this->sudoku = sudoku;
    this->settings = settings;

    for(int i = 0; i < 10; i++){
        this->numberState.push_back(0);
    }
}

Cell * SudokuClass::getCell(int posX, int posY)
{
    return this->sudoku->getSudoku()[posX-1][posY-1];
}

void SudokuClass::defaultColors(){
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++){
            Cell * cell = this->sudoku->getSudoku()[i][j];
            cell->setDefColor();
            cell->setDefFontColor();
        }
}

std::vector<int> SudokuClass::updateNumberState()
{
    for(int i = 0; i < 10;i++){
        this->numberState[i] = 0;
    }
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++){
            Cell * cell = this->sudoku->getSudoku()[i][j];
            this->numberState[cell->getVal()]++;
        }
    return this->numberState;
}

void SudokuClass::clickCell(int posX, int posY)
{
    //kliknięcie na komórkę. Podświetlenie kolumn oraz wierszy. Jeśli została kliknięta cyfra - zostają podświetlone te same cyfry w sudoku
    Cell * cell = this->getCell(posX, posY);
    this->defaultColors();
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            Cell * editedCell = this->sudoku->getSudoku()[i][j];
            if(editedCell != cell)
                editedCell->setClicked(false);
            if(((i + 1 == cell->getRow()) || (j + 1 == cell->getCol())) &&
                    (((i + 1 == cell->getRow()) && (j + 1 == cell->getCol())) || (this->settings->ColsAndRows))){
                editedCell->setCellColor(QColor(0, 154, 255, 255));
            }
            if((cell->getVal() != 0) && (editedCell->getVal() == cell->getVal()) && (this->settings->Numbers)){
                editedCell->setFontColor(QColor(Qt::red));
            }
        }
    }
}

void SudokuClass::numberClicked(int number)
{
    //kliknięty numer na panelu dolnym. Podświetlenie tych samych numerów na planszy
    if((number < 1) || (number > 9)) return;
    this->defaultColors();
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            Cell * cell = this->sudoku->getSudoku()[i][j];
            if(cell->getVal() == number){
                cell->setFontColor(QColor(Qt::red));
            }
        }
    }
}

void SudokuClass::clearMemory()
{
    delete this->sudoku;
}

void SudokuClass::updateCellVal(int posX, int posY)
{
    if(!this->settings->Errors) {
        this->clickCell(posX, posY);
        return;
    }
    //zamknięcie okienka
    bool rowConflict = false;
    bool colConflict = false;
    Cell * cell = this->getCell(posX, posY);
    //this->defaultColors();
    for(int i = 0; i < 9; i++)
        for(int j = 0; j <9; j++){
            Cell * tCell = this->sudoku->getSudoku()[i][j];
            if((cell->getVal() != 0) && (cell->getVal() == tCell->getVal()) && (cell != tCell)){
                if(cell->getCol() == tCell->getCol()) colConflict = true;
                if(cell->getRow() == tCell->getRow()) rowConflict = true;
            }
        }
    if(rowConflict || colConflict){
        for(int i = 0; i < 9; i++)
            for(int j = 0; j <9; j++){
                Cell * tCell = this->sudoku->getSudoku()[i][j];
                if(rowConflict && (cell->getRow() - 1 == i))
                    tCell->setCellColor(QColor(Qt::red));
                if(colConflict && (cell->getCol() - 1 == j))
                    tCell->setCellColor(QColor(Qt::red));
            }
    }else{
        this->clickCell(posX, posY);
    }
}

bool SudokuClass::checkSudoku()
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j <9; j++){
            Cell * actualCell = this->sudoku->getSudoku()[i][j];
            Cell * solvedCell = this->sudoku->getSolvedSudoku()[i][j];
            if(actualCell->getVal() != solvedCell->getVal()){
                return false;
            }
        }
    return true;
}

SudokuStructure *SudokuClass::getSudokuStructure()
{
    return this->sudoku;
}

std::vector<int> SudokuClass::getNumberState()
{
    return this->numberState;
}

Cell *SudokuClass::getHint()
{
    this->defaultColors();
    bool searching = true;
    while(searching){
        srand((unsigned)time(NULL));
        int i = rand() % 9;
        int j = rand() % 9;
        Cell * cell = this->sudoku->getSudoku()[i][j];
        if(cell->getVal() == 0){
            Cell * solvedCell = this->sudoku->getSolvedSudoku()[i][j];
            cell->setVal(solvedCell->getVal());
            cell->setCellColor(QColor(Qt::green));
            searching = false;
            return cell;
        }
    }
    return NULL;
}
