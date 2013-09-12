#include "HelperClass.h"
#include <vector>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "Cell.h"
#include "SudokuStructure.h"
#include <QFileDialog>
#include <QMessageBox>

SudokuPuzzle HelperClass::getSudokuPuzzleFromTable(int sudoku[])
{
    SudokuPuzzle sudokuPuzzle;
    for(int i = 0; i < 9; i++){
        std::vector<Cell *> rowTable;
        sudokuPuzzle.push_back(rowTable);
    }
    int row = 0;
    int col = 0;
    for(int i = 0; i < 81; i++){
        if((i % 9 == 0)){
            col = 0;
            row++;
        }
        col++;
        bool fixed = false;
        if(sudoku[i] > 0) fixed = true;
        Cell * cell = new Cell(row,col,sudoku[i], fixed);
        sudokuPuzzle.at(row-1).push_back(cell);

    }

    return sudokuPuzzle;
}

void HelperClass::saveSudokuPuzzleToFile(SudokuStructure *ss)
{
    QString filename = QFileDialog::getSaveFileName(
                NULL,
                "Zapisz sudoku do pliku",
                QDir::currentPath(),
                "Sudoku file (*.sdu)" );
    if( !filename.isNull() )
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++){
                Cell * cell = ss->getOriginalSudoku()[i][j];
                out << QString::number(cell->getVal());
            }
        out << "|";
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++){
                Cell * cell = ss->getSolvedSudoku()[i][j];
                out << QString::number(cell->getVal());
            }
        out << "\n";
        file.close();
    }
}

SudokuStructure * HelperClass::loadSudokuFromFile(){
    QString filename = QFileDialog::getOpenFileName(
                NULL,
                "Wczytaj sudoku z pliku",
                QDir::currentPath(),
                "Sudoku file (*.sdu)"
                );
    if(!filename.isNull()){
        //wczytywanie z pliku
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            QStringList strings;
            while(!in.atEnd()){
                strings += in.readLine().split("|");
            }
            if(strings.length() != 2){
                QMessageBox::critical(NULL, "Błąd!", "Plik posiada nieprawidłowy format");
                return NULL;
            }
            QString firstSudoku = strings.takeFirst();
            int sudoku[81];
            for(int i = 0; i < firstSudoku.length(); i++){
                sudoku[i] = firstSudoku[i].digitValue();
            }
            int solution[81];
            QString secondSodoku = strings.takeFirst();
            for(int i = 0; i < secondSodoku.length(); i++){
                solution[i] = secondSodoku[i].digitValue();
            }
            SudokuPuzzle sudokuPuzzle = HelperClass::getSudokuPuzzleFromTable(sudoku);
            SudokuPuzzle originalSudoku = HelperClass::getSudokuPuzzleFromTable(sudoku);
            SudokuPuzzle solvedSudoku = HelperClass::getSudokuPuzzleFromTable(solution);
            SudokuStructure * ss = new SudokuStructure(sudokuPuzzle, solvedSudoku, originalSudoku);
            file.close();
            return ss;
        }else{
            QMessageBox::critical(NULL, "Błąd!", "Plik o podanej nazwie nie istnieje bądź nie ma uprawnień do jego odczytu");
            return NULL;
        }
    }else{
        return NULL;
    }
}
